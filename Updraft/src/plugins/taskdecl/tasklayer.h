#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKLAYER_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKLAYER_H_

#include <QWidget>
#include "taskfile.h"

namespace osg {
  class Node;
  class Group;
}

namespace osgEarth {
namespace Util {
  class ObjectPlacer;
}
}

namespace Updraft {

class MapLayerInterface;
class TabInterface;
class TaskDeclaration;

/// Class storing a task layer.
class TaskLayer : public QObject {
  Q_OBJECT

 public:
  TaskLayer(bool displayed_, TaskDeclaration *plugin_,
    TaskFile *file_, int _newTaskIndex);

  virtual ~TaskLayer();

  /// \return osgNode associated with the layer.
  osg::Node* getNode() const;

  /// \return Display state
  bool isDisplayed();

  /// Changes display state.
  /// \param displayed_ new value of a display state
  void display(bool displayed_);

  /// Creates layer title with respect to file existence
  /// and save flag (star).
  /// \return Layer title
  QString getTitle() const;

  /// \return State of task file storage
  TaskFile::StorageState getStorageState() const;

  /// \return Index displayed in new task title
  int getNewTaskIndex() const;

 public slots:
  void mapLayerDisplayed(bool value, MapLayerInterface* sender);

  /// Slot that gets called when the layer is about to close.
  void tryCloseLayer();

 private:
  /// TaskDeclaration plugin
  TaskDeclaration *plugin;

  /// MapLayer object associated with task
  Updraft::MapLayerInterface* mapLayer;

  /// osg Node representing this task layer
  osg::Group* group;

  /// Tab in bottom pane
  TabInterface* tab;

  /// Task data object
  TaskFile *file;

  /// Display state of task graphics
  bool displayed;

  /// Number which is displayed in title of new task
  int newTaskIndex;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKLAYER_H_
