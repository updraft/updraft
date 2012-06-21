#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKLAYER_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKLAYER_H_

#include <QWidget>
#include "taskfile.h"
#include "util/location.h"

namespace osg {
  class Node;
  class Group;
  class Geode;
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
class TaskDeclPanel;
struct TurnPoint;

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

  /// \return TaskFile object for this task
  TaskFile* getTaskFile();

  /// \return Task declaration GUI panel.
  TaskDeclPanel* getTaskDeclPanel();

  /// \return Whether this layer's tab is selected.
  bool isTabSelected();

  /// Creates a new task point from a turn-point.
  void newTaskPoint(const TurnPoint* tp);

  /// Creates a new task point on the map.
  void newTaskPoint(const Util::Location& loc);

  /// Saves file. If the path is not set, file dialog is invoked.
  /// \return True on success. False on fail or cancell.
  bool save();

  /// Moves forward in file history.
  void undo();

  /// Moves back in file history.
  void redo();

  /// Invokes save as dialog and saves file.
  /// \return True on success. False on fail or cancell.
  bool saveAs();

  /// Ask the layer if closing it is Ok.
  /// This method may display a dialog window asking the user
  /// and based on the result of this dialog save the file.
  bool askClose();

 public slots:
  void mapLayerDisplayed(bool value, MapLayerInterface* sender);

  /// Slot that gets called when the layer is about to close.
  void tryCloseLayer();

  /// Slot that should be called when the layer's tab is selected
  void tabSelected();

  /// Slot that should be called when the layer's tab is selected
  void tabDeselected();

  /// Slot catching signal on task update. It updates osg scene.
  void taskDataChanged();

  /// Slot catching signal on task storage state change.
  /// It updates label in both bottom tab and left pane.
  void taskStorageStateChanged();

 private:
  /// Draws task lines.
  /// \param geode target geode object.
  /// Lines are added as drawables into geode.
  void drawLines(osg::Geode *geode);

  /// Draw curtain of the task line.
  /// \param geode target geode object.
  void drawCurtain(osg::Geode *geode);

  /// Draw TP areas of the task.
  /// \param geode target group object.
  void drawAreas(osg::Group *group);

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

  /// Task GUI panel
  TaskDeclPanel* panel;

  /// Display state of task graphics
  bool displayed;

  /// Selection state of the task's GUI tab
  bool tabSelectedState;

  /// Number which is displayed in title of new task
  int newTaskIndex;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKLAYER_H_
