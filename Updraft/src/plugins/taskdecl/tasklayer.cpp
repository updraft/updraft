#include <osg/Group>
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

  // Create new mapLayer in mapLayerGroup, assign osgNode and title.
  mapLayer = plugin->mapLayerGroup->insertMapLayer(getNode(), getTitle(), -1);

  mapLayer->connectSignalDisplayed(this,
    SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));

  tab->connectSignalCloseRequested(this, SLOT(tryCloseLayer()));
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
  TaskData* tData = file->beginEdit();
  TaskPoint* newPoint = new TaskPoint();
  newPoint->setTP(tp);
  tData->insertTaskPoint(newPoint, tpIndex);
  file->endEdit(false);  // TODO(cestmir): Create redo/undo

  // Modify GUI
  panel->newTurnpointButton(tpIndex, tp->name);
  panel->newAddTpButton(tpIndex + 1, true);
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

}  // End namespace Updraft
