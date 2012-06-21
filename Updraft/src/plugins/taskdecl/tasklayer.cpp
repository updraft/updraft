#include <osg/Depth>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osgEarthUtil/ObjectPlacer>
#include <osgEarthSymbology/MeshSubdivider>
#include "tasklayer.h"
#include "coreinterface.h"
#include "tabinterface.h"
#include "taskdeclpanel.h"
#include "taskdeclaration.h"
#include "taskdata.h"
#include "taskpoint.h"
#include "../turnpoints/turnpoint.h"
#include "pluginbase.h"

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
  panel = new TaskDeclPanel(this);
  tab = g_core->createTab(panel, getTitle());

  // Connect tab's signals onto the taskLayer
  tab->connectSignalSelected(this, SLOT(tabSelected()));
  tab->connectSignalDeselected(this, SLOT(tabDeselected()));

  // Initialize the gui from the file if needed
  if (file) panel->initFromFile(file);

  // Create new mapLayer in mapLayerGroup, assign osgNode and title.
  mapLayer = plugin->mapLayerGroup->createMapLayer(getNode(), getTitle(), -1);

  // Connect display and close signals
  mapLayer->connectSignalChecked(this,
    SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
  tab->connectSignalCloseRequested(this, SLOT(tryCloseLayer()));

  // Connect the dataChanged signal
  connect(this->file, SIGNAL(dataChanged()),
    this->panel, SLOT(updateButtons()));
  connect(file, SIGNAL(dataChanged()), this, SLOT(taskDataChanged()));

  // Connect the storageStateChanged signal
  connect(file, SIGNAL(storageStateChanged()),
    this, SLOT(taskStorageStateChanged()));

  mapLayer->zoom();

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
      return "[" + tr("New Task") + QString(" %1]").arg(newTaskIndex);

    case TaskFile::UNSTORED_EDITED:
      return "[" + tr("New Task") + QString(" %1] *").arg(newTaskIndex);

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

// TODO(cestmir): Remove redundancy in following two methods
void TaskLayer::newTaskPoint(const TurnPoint* tp) {
  int tpIndex = panel->getToggledButtonIndex();
  if (tpIndex < 0) return;

  // Modify the file data
  TaskData* tData = file->beginEdit(true);
  TaskPoint* newPoint = new TaskPoint();
  newPoint->setTP(tp);

  // If the task point insertion failed, remove it
  if (!tData->insertTaskPoint(newPoint, tpIndex)) {
    delete newPoint;
  }

  file->endEdit();
}

void TaskLayer::newTaskPoint(const Util::Location& loc) {
  int tpIndex = panel->getToggledButtonIndex();
  if (tpIndex < 0) return;

  // Modify the file data
  TaskData* tData = file->beginEdit(true);
  TaskPoint* newPoint = new TaskPoint();
  newPoint->setLocation(loc);

  newPoint->setName(
    tr("Map location:\n%1\n%2").arg(loc.latToString()).arg(loc.lonToString()));

  // If the task point insertion failed, remove it
  if (!tData->insertTaskPoint(newPoint, tpIndex)) {
    delete newPoint;
  }

  file->endEdit();
}

bool TaskLayer::save() {
  switch (file->getStorageState()) {
    case TaskFile::UNSTORED_EMPTY:
    case TaskFile::UNSTORED_EDITED:
      return saveAs();

    case TaskFile::STORED_DIRTY:
    case TaskFile::STORED_SYNCHRONIZED:
      file->save();
      break;
  }

  return true;
}

void TaskLayer::undo() {
  file->undo();
}

void TaskLayer::redo() {
  file->redo();
}

bool TaskLayer::saveAs() {
  QString filePath = QFileDialog::getSaveFileName(panel, tr("Save Task As"),
    QString(), tr("Task file") + " (*.tsk)");

  if (filePath.length() > 0) {
    file->saveAs(filePath);
    return true;
  }

  return false;
}

void TaskLayer::mapLayerDisplayed(bool value, MapLayerInterface* sender) {
  if (sender != mapLayer) {
    return;
  }

  // First try to display/enable layer in TaskDeclaration plugin.
  // Then if it is displayed, show it also in the map.
  display(value);
  mapLayer->setVisibility(isDisplayed());
}

bool TaskLayer::askClose() {
  // When file is modified, ask for save.
  if (file->getStorageState() != TaskFile::UNSTORED_EDITED
    && file->getStorageState() != TaskFile::STORED_DIRTY) {
    return true;
  }

  QMessageBox msgBox;
  msgBox.setText(tr("The document %1 has been modified.").arg(getTitle()));
  msgBox.setInformativeText(tr("Do you want to save your changes?"));
  msgBox.setStandardButtons(QMessageBox::Save |
    QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);

  // Show message box.
  switch (msgBox.exec()) {
    case QMessageBox::Save:
      // When select save and it is not succesfull, refuse closing.
      return save();
    case QMessageBox::Discard:
      return true;
    default:
      return false;
  }
}

void TaskLayer::tryCloseLayer() {
  if (!askClose()) {
    return;
  }

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
  drawLines(geodeLines);
  group->addChild(geodeLines);

  osg::Geode *curtain = new osg::Geode();
  drawCurtain(curtain);
  group->addChild(curtain);
}

void TaskLayer::taskStorageStateChanged() {
  tab->setTitle(getTitle());
  mapLayer->setTitle(getTitle());
}

void TaskLayer::drawLines(osg::Geode *geode) {
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

  const osg::EllipsoidModel* ellipsoid = g_core->getCurrentMapEllipsoid();

  // Reads all task points and fills draw array.
  int pointIndex = 0;
  const TaskPoint *point = NULL;
  while ((point = taskData->getTaskPoint(pointIndex))) {
    // TODO(Tom): correct altitude
    double x, y, z;
    ellipsoid->convertLatLongHeightToXYZ(
      point->getLocation().lat_radians(),
      point->getLocation().lon_radians(),
      point->getLocation().alt + 1000.0,
      x, y, z);

    vertexData->push_back(osg::Vec3(x, y, z));
    ++pointIndex;
  }

  file->endRead();

  drawArrayLines->setFirst(0);
  drawArrayLines->setCount(vertexData->size());

  // Subdivides lines
  osgEarth::Symbology::MeshSubdivider meshSubdivider;
  meshSubdivider.run(*geom, 0.01, osgEarth::GEOINTERP_GREAT_CIRCLE);

  // Sets lines appearance.

  osg::StateSet* stateSet = geode->getOrCreateStateSet();

  // Turns off lighting.
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

  // Sets line width.
  osg::LineWidth* linewidth = new osg::LineWidth();
  linewidth->setWidth(5.0);
  stateSet->setAttributeAndModes(linewidth, osg::StateAttribute::ON);

  // Sets line color.
  osg::Vec4Array* colors = new osg::Vec4Array;
  colors->push_back(osg::Vec4(0.78, 0.64, 0.0, 1.0));
  geom->setColorArray(colors);
  geom->setColorBinding(osg::Geometry::BIND_OVERALL);
}

void TaskLayer::drawCurtain(osg::Geode *geode) {
  osg::Geometry* geom = new osg::Geometry();

  geode->addDrawable(geom);

  osg::DrawArrays* drawArray =
    new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP);
  geom->addPrimitiveSet(drawArray);

  osg::Vec3Array* vertexData = new osg::Vec3Array();
  geom->setVertexArray(vertexData);

  // Loads TaskData.
  const TaskData *taskData = file->beginRead();
  if (taskData == NULL) {
    return;
  }

  const osg::EllipsoidModel* ellipsoid = g_core->getCurrentMapEllipsoid();

  // Reads all task points and fills draw array.
  int pointIndex = 0;
  const TaskPoint *point = NULL;
  while ((point = taskData->getTaskPoint(pointIndex))) {
    double x, y, z;

    ellipsoid->convertLatLongHeightToXYZ(
      point->getLocation().lat_radians(),
      point->getLocation().lon_radians(),
      point->getLocation().alt + 1000.0,
      x, y, z);
    vertexData->push_back(osg::Vec3(x, y, z));

    ellipsoid->convertLatLongHeightToXYZ(
      point->getLocation().lat_radians(),
      point->getLocation().lon_radians(),
      0,
      x, y, z);
    vertexData->push_back(osg::Vec3(x, y, z));
    ++pointIndex;
  }

  file->endRead();

  drawArray->setFirst(0);
  drawArray->setCount(vertexData->size());

  // Subdivides lines
  osgEarth::Symbology::MeshSubdivider meshSubdivider;
  meshSubdivider.run(*geom, 0.01, osgEarth::GEOINTERP_GREAT_CIRCLE);

  geom->setColorBinding(osg::Geometry::BIND_OVERALL);

  osg::Vec4Array* color = new osg::Vec4Array();
  color->push_back(osg::Vec4(0.5, 0.5, 0.5, 0.5));
  geom->setColorArray(color);

  osg::StateSet* stateSet = geode->getOrCreateStateSet();
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
  stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
  stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

  osg::Depth* depth = new osg::Depth;
  depth->setWriteMask(false);
  stateSet->setAttributeAndModes(depth, osg::StateAttribute::ON);

  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
}

}  // End namespace Updraft
