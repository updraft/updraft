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

class PickHandler: public osgGA::GUIEventHandler {
  Q_DECLARE_TR_FUNCTIONS(PickHandler)

 public:
  PickHandler();

  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

 protected:
  // Store mouse xy location for button press & move events.
  float mX, mY;

  void raiseLeftClick(QVector<Pair> mapObjects);
  void raiseRightClick(QVector<Pair> mapObjects);
  MapObject* getMapObject(osg::Node* node);

  // Says whether two mouse events are close enough to belong together
  bool eventsCloseEnough(float x1, float y1, float x2, float y2);

  SettingInterface* mouseEventTolerance;

  // Get all MapObjectes associated with intersected nodes.
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

