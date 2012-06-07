#ifndef UPDRAFT_SRC_CORE_PICKHANDLER_H_
#define UPDRAFT_SRC_CORE_PICKHANDLER_H_

#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <QtGui>
#include "../mapobject.h"

namespace Updraft {

class SettingInterface;

namespace Core {

typedef QPair<MapObject*, osg::Vec3> Pair;

/// Handler of osg events that implements clicking of map objects.
class PickHandler: public osgGA::GUIEventHandler {
  Q_DECLARE_TR_FUNCTIONS(PickHandler)

 public:
  PickHandler();

  /// Handles the osg gui event.
  /// It gets all of the intersected map objects and then, according to the
  /// event type, either a left click or right click is raised.
  /// \param ea The osg event adapter for the event that is being handled.
  /// \param aa The osg action adapter for the current event. This is not used
  ///           in thecurrent implementation.
  /// \return true If the event was accepted and handled.
  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

 protected:
  /// Store mouse xy location for button press & move events.
  float mX, mY;

  /// Method that calls the click handlers for all interested plugins for all
  /// intersected map objects.
  /// \param mapObjects The vector of intersected map objects.
  void raiseLeftClick(QVector<Pair> mapObjects);

  /// Requests the context menu actions from all plugins for all intersected
  /// map objects. The context menu is then executed.
  /// \param mapObjects The vector of intersected map objects.
  void raiseRightClick(QVector<Pair> mapObjects);
  MapObject* getMapObject(osg::Node* node);

  /// Says whether two mouse events are close enough to belong together
  /// \param x1 X-coordinate of the first event
  /// \param y1 Y-coordinate of the first event
  /// \param x2 X-coordinate of the second event
  /// \param y2 Y-coordinate of the second event
  /// \return True if the events are close enough.
  bool eventsCloseEnough(float x1, float y1, float x2, float y2);

  /// The setting for the mouse event tolerance used in the method
  /// eventsCloseEnough()
  SettingInterface* mouseEventTolerance;

  /// Get all MapObjectes associated with nodes intersected by a click.
  /// \param x The normalized X-coordinate of the mouse click
  /// \param y The normalized Y-coordinate of the mouse click
  /// \param viewer The viewer currently used to render the scene
  /// \return QVector of all map objects that were intersected by the click
  ///                 action with the given normalized coordinates.
  QVector<Pair> getIntersectedMapObjects(
    const double x,
    const double y,
    osgViewer::Viewer* viewer);

  // List of all owned QActions that is used for destruction of these actions.
  QList<QAction*> ownedActions;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PICKHANDLER_H_

