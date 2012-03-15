#include "pickhandler.h"
#include "scenemanager.h"
#include "updraft.h"
#include "../eventinfo.h"
#include "../pluginbase.h"

namespace Updraft {
namespace Core {

bool PickHandler::handle(
  const osgGA::GUIEventAdapter& ea,
  osgGA::GUIActionAdapter& aa) {
  osgViewer::Viewer* viewer = static_cast<osgViewer::Viewer*>(&aa);
  if (!viewer || !viewer->getSceneData())
    return false;

  switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::PUSH:
    case osgGA::GUIEventAdapter::MOVE:
      // Record mouse location for the button press
      // and move events.
      mX = ea.getX();
      mY = ea.getY();
      return false;
    case osgGA::GUIEventAdapter::RELEASE:
      // If the mouse hasn't moved since the last
      // button press or move event, perform a
      // pick. (Otherwise, the trackball
      // manipulator will handle it.)
      if (mX == ea.getX() && mY == ea.getY()) {
        // click event
        QVector<MapObject*> mapObjects = getIntersectedMapObjects(
          ea.getXnormalized(), ea.getYnormalized(), viewer);

        if (mapObjects.empty())
          return false;

        raiseClickEvent(mapObjects);
        return true;
      }
      return false;
    default:
      return false;
  }
}

void PickHandler::raiseClickEvent(QVector<MapObject*> mapObjects) {
  foreach(MapObject* mapObject, mapObjects) {
    if (!mapObject) continue;

    EventInfo eventInfo(LEFT_CLICK);

    foreach(PluginBase* plugin, updraft->pluginManager->getAllPlugins()) {
      plugin->handleMouseEvent(mapObject, &eventInfo);
    }
  }
}

MapObject* PickHandler::getMapObject(osg::Node* node) {
  return updraft->sceneManager->getNodeMapObject(node);
}

QVector<MapObject*> PickHandler::getIntersectedMapObjects(
  const double x,
  const double y,
  osgViewer::Viewer* viewer ) {
  // TODO(bohdan): settings? increase in interations?
  // (to improve tolerance without crippling accuracy)
  double w(.05), h(.05);

  osgUtil::PolytopeIntersector* picker =
    new osgUtil::PolytopeIntersector(
      osgUtil::Intersector::PROJECTION, x-w, y-h, x+w, y+h);

  osgUtil::IntersectionVisitor iv(picker);
  viewer->getCamera()->accept(iv);

  osgUtil::PolytopeIntersector::Intersections& intersections =
    picker->getIntersections();

  QVector<MapObject*> mapObjects;

  std::set<osgUtil::PolytopeIntersector::Intersection>::iterator it;
  for (it = intersections.begin(); it != intersections.end(); ++it) {
    const osg::NodePath& nodePath = it->nodePath;
    unsigned int idx = nodePath.size();
    while (idx--) {
      MapObject* mapObject = getMapObject(nodePath[idx]);
      if (mapObject != NULL) {
        mapObjects.append(mapObject);
      }
    }
  }

  return mapObjects;
}

}  // End namespace Core
}  // End namespace Updraft

