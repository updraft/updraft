#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osgEarthUtil/ObjectPlacer>
#include "tasklayer.h"
#include "coreinterface.h"
#include "tabinterface.h"
#include "taskdeclpanel.h"
#include "taskdeclaration.h"
#include "taskdata.h"
#include "taskpoint.h"
#include "../turnpoints/turnpoint.h"

namespace Updraft {

TaskLayer::TaskLayer(bool displayed_, TaskDeclaration *plugin_,
  TaskFile *file_,
  int _newTaskIndex)
  : plugin(plugin_),
  mapLayer(NULL),
  group(new osg::Group()),
  file(file_),
  displayed(displayed_),
  tabSelectedState(true),
  newTaskIndex(_newTaskIndex) {
  // Create new tab in bottom pane.
  panel = new TaskDeclPanel();
  panel->setTaskLayer(this);
  tab = plugin->core->createTab(panel, getTitle());

  // Connect tab's signals onto the taskLayer
  tab->connectSignalSelected(this, SLOT(tabSelected()));
  tab->connectSignalDeselected(this, SLOT(tabDeselected()));

  // Initialize the gui from the file if needed
  if (file) panel->initFromFile(file);

  // Create new mapLayer in mapLayerGroup, assign osgNode and title.
  mapLayer = plugin->mapLayerGroup->insertMapLayer(getNode(), getTitle(), -1);

  // Connect display and close signals
  mapLayer->connectSignalDisplayed(this,
    SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
  tab->connectSignalCloseRequested(this, SLOT(tryCloseLayer()));

  // Connect the dataChanged signal
  connect(this->file, SIGNAL(dataChanged()),
    this->panel, SLOT(updateButtons()));
  connect(file, SIGNAL(dataChanged()), this, SLOT(taskDataChanged()));

  // Connect the storageStateChanged signal
  connect(file, SIGNAL(storageStateChanged()),
    this, SLOT(taskStorageStateChanged()));

  taskDataChanged();
}

TaskLayer::~TaskLayer() {
  plugin->mapLayerGroup->removeMapLayer(mapLayer);
  tab->close();
  delete file;
}

osg::Node* TaskLayer::getNode() const {
  return group;
}

bool TaskLayer::isDisplayed() {
  return displayed;
}

void TaskLayer::display(bool displayed_) {
  displayed = displayed_;
}

QString TaskLayer::getTitle() const {
  switch (file->getStorageState()) {
    case TaskFile::UNSTORED_EMPTY:
      return QString("[New Task %1]").arg(newTaskIndex);

    case TaskFile::UNSTORED_EDITED:
      return QString("[New Task %1] *").arg(newTaskIndex);

    case TaskFile::STORED_SYNCHRONIZED:
      return file->getFileName();

    case TaskFile::STORED_DIRTY:
      return file->getFileName() + " *";

    default:
      return "";
  }
}

TaskFile::StorageState TaskLayer::getStorageState() const {
  return file->getStorageState();
}

int TaskLayer::getNewTaskIndex() const {
  return newTaskIndex;
}

TaskFile* TaskLayer::getTaskFile() {
  return file;
}

TaskDeclPanel* TaskLayer::getTaskDeclPanel() {
  return panel;
}

bool TaskLayer::isTabSelected() {
  return tabSelectedState;
}

void TaskLayer::newTaskPoint(const TurnPoint* tp) {
  int tpIndex = panel->getToggledButtonIndex();
  if (tpIndex < 0) return;

  // Modify the file data
  TaskData* tData = file->beginEdit(true);
  TaskPoint* newPoint = new TaskPoint();
  newPoint->setTP(tp);
  tData->insertTaskPoint(newPoint, tpIndex);
  file->endEdit();
}

void TaskLayer::save() {
  switch (file->getStorageState()) {
    case TaskFile::UNSTORED_EMPTY:
    case TaskFile::UNSTORED_EDITED:
      saveAs();
      break;

    case TaskFile::STORED_DIRTY:
    case TaskFile::STORED_SYNCHRONIZED:
      file->save();
      break;
  }
}

void TaskLayer::undo() {
  file->undo();
}

void TaskLayer::redo() {
  file->redo();
}

void TaskLayer::saveAs() {
  QString filePath = QFileDialog::getSaveFileName(panel, "Save Task As",
    QString(), QString("Task File (*.tsk)"));

  if (filePath.length() > 0) {
    file->saveAs(filePath);
  }
}

void TaskLayer::mapLayerDisplayed(bool value, MapLayerInterface* sender) {
  if (sender != mapLayer) {
    return;
  }

  // First try to display/enable layer in TaskDeclaration plugin.
  // Then if it is displayed, show it also in the map.
  display(value);
  mapLayer->setVisible(isDisplayed());
}

void TaskLayer::tryCloseLayer() {
  // TODO(Tom): Ask for close, allow refuse.

  // Removes layer from list in plugin.
  TTaskLayerList::iterator itLayer = plugin->layers.begin();
  for (; itLayer != plugin->layers.end(); ++itLayer) {
    if ((*itLayer) == this) {
      break;
    }
  }
  if (itLayer != plugin->layers.end())
    plugin->layers.erase(itLayer);

  delete this;
}

void TaskLayer::tabSelected() {
  tabSelectedState = true;
}

void TaskLayer::tabDeselected() {
  tabSelectedState = false;
}

void TaskLayer::taskDataChanged() {
  // Clears content of group node. (Erases old scene)
  group->removeChildren(0, group->getNumChildren());

  // Draws lines, adds them to group.
  osg::Geode *geodeLines = new osg::Geode();
  DrawLines(geodeLines);
  group->addChild(geodeLines);
}

void TaskLayer::taskStorageStateChanged() {
  tab->setTitle(getTitle());
  plugin->mapLayerGroup->setMapLayerTitle(mapLayer, getTitle());
}

void TaskLayer::DrawLines(osg::Geode *geode) {
  // Creates geometry object and draw array.
  osg::Geometry* geom = new osg::Geometry();
  geode->addDrawable(geom);

  osg::DrawArrays* drawArrayLines =
    new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);

  geom->addPrimitiveSet(drawArrayLines);

  osg::Vec3Array* vertexData = new osg::Vec3Array();
  geom->setVertexArray(vertexData);

  // Loads TaskData.
  const TaskData *taskData = file->beginRead();
  if (taskData == NULL) {
    return;
  }

  osgEarth::Util::ObjectPlacer *objectPlacer =
    plugin->mapLayerGroup->getObjectPlacer();

  // Reads all task points and fills draw array.
  int pointIndex = 0;
  const TaskPoint *point = NULL;
  while (point = taskData->getTaskPoint(pointIndex)) {
    osg::Matrixd matrix;

    // TODO(Tom): correct altitude
    objectPlacer->createPlacerMatrix(point->getLocation().lat,
      point->getLocation().lon, point->getLocation().alt + 1000.0, matrix);

    vertexData->push_back(osg::Vec3(0.0, 0.0, 0.0) * matrix);
    ++pointIndex;
  }

  file->endRead();

  drawArrayLines->setFirst(0);
  drawArrayLines->setCount(vertexData->size());

  // Sets lines appearance.

  osg::StateSet* stateSet = geode->getOrCreateStateSet();

  // Turns off lighting.
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

  // Sets line width.
  osg::LineWidth* linewidth = new osg::LineWidth();
  linewidth->setWidth(4.0);
  stateSet->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
}

}  // End namespace Updraft
