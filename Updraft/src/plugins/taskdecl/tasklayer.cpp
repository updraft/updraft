#include <osg/Group>
#include "tasklayer.h"
#include "coreinterface.h"
#include "tabinterface.h"
#include "taskdeclpanel.h"
#include "taskdeclaration.h"

namespace Updraft {

TaskLayer::TaskLayer(bool displayed_, TaskDeclaration *plugin_,
  TaskFile *file_,
  int _newTaskIndex)
  : plugin(plugin_),
  mapLayer(NULL),
  group(new osg::Group()),
  file(file_),
  displayed(displayed_),
  newTaskIndex(_newTaskIndex) {
  // Create new tab in bottom pane.
  tab = plugin->core->createTab(new TaskDeclPanel(), getTitle());

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

}  // End namespace Updraft
