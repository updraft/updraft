#include "pickhandler.h"

#include <QSet>
#include <QPair>
#include "scenemanager.h"
#include "../settinginterface.h"
#include "settingsmanager.h"
#include "updraft.h"
#include "pickaction.h"
#include "../eventinfo.h"
#include "../pluginbase.h"
#include "ui/mainwindow.h"
#include "pluginmanager.h"

namespace Updraft {
namespace Core {

PickHandler::PickHandler(): mX(0), mY(0) {
  mouseEventTolerance = updraft->settingsManager->addSetting(
    "map_advanced:mouseEventTolerance",
    tr("Mouse click tolerance"),
    QVariant(10.0),
    true);
}

bool PickHandler::handle(
  const osgGA::GUIEventAdapter& ea,
  osgGA::GUIActionAdapter& aa) {
  osgViewer::Viewer* viewer = static_cast<osgViewer::Viewer*>(&aa);
  if (!viewer || !viewer->getSceneData())
    return false;

  switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::PUSH:
      // Record mouse location for the button press events
      mX = ea.getX();
      mY = ea.getY();
      return false;
    case osgGA::GUIEventAdapter::RELEASE:
      // If the mouse hasn't moved since the last
      // button press or move event, perform a
      // pick. (Otherwise, the trackball
      // manipulator will handle it.)
      if (eventsCloseEnough(mX, mY, ea.getX(), ea.getY())) {
        // click event
        QVector<Pair> mapObjects =
          getIntersectedMapObjects(ea.getXnormalized(), ea.getYnormalized(),
          viewer);

        if (mapObjects.empty())
          return false;

        switch (ea.getButton()) {
          case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
            raiseLeftClick(mapObjects);
          return true;
          case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
            raiseRightClick(mapObjects);
          return true;
        }
        return false;
      }
      return false;
    default:
      return false;
  }
}

void PickHandler::raiseLeftClick(
  QVector<Pair> mapObjects) {
  qDebug("--- Left click event ---");

  Menu* moMenu = updraft->mainWindow->getSystemMenu(MENU_MAPOBJECT);
  moMenu->clear();

  // Create the PickActions and prevent double adding of objects
  QSet<MapObject*> alreadyAdded;
  for (int i = 0; i < mapObjects.size(); i++) {
    Pair mapObjectPair = mapObjects[i];
    if (!mapObjectPair.first) continue;

    if (alreadyAdded.contains(mapObjectPair.first)) continue;
    alreadyAdded.insert(mapObjectPair.first);

    EventInfo evt(LEFT_CLICK, mapObjectPair.second);

    qDebug("=================");
    qDebug() << mapObjectPair.second.x();
    qDebug() << mapObjectPair.second.y();
    qDebug() << mapObjectPair.second.z();

    // Insert the plugins into PickAction only if they want to handle the event
    PickAction* action = new PickAction(mapObjectPair.first, evt);
    foreach(PluginBase* plugin, updraft->pluginManager->getAllPlugins()) {
      if (plugin->wantsToHandleClick(mapObjectPair.first)) {
        action->pushPlugin(plugin);
      }
    }

    // Now insert the action into menu only if a plugin wants to handle the ev.
    if (!action->isEmpty()) {
      moMenu->appendAction(action);
    } else {
      delete action;  // Noone owns the action so we have to destroy it
    }
  }

  // Show the menu if more than one action was inserted
  if (moMenu->getQMenu()->actions().size()> 1) {
    QWidget* mapWidget = updraft->mainWindow->getMapWidget();
    int mh = mapWidget->height();

    // The coordinates have to be Y-inverted and mapped to screen
    moMenu->getQMenu()->exec(mapWidget->mapToGlobal(QPoint(mX, mh - mY)));
  // If there was only one inserted action, trigger it
  } else if (moMenu->getQMenu()->actions().size() == 1) {
    moMenu->getQMenu()->actions().first()->trigger();
  }
}

void PickHandler::raiseRightClick(
  QVector<Pair> mapObjects) {
  qDebug("--- Right click event ---");

  // Clear the previous menu
  Menu* moMenu = updraft->mainWindow->getSystemMenu(MENU_MAPOBJECT);
  moMenu->clear();

  // Delete all the old actions
  foreach(QAction* action, ownedActions) {
    delete action;
  }
  ownedActions.clear();

  // Submenu actions for individual map objects
  QList<QAction*> submenuActions;

  // Check for duplicate map objects
  QSet<MapObject*> alreadyAdded;
  for (int i = 0; i < mapObjects.size(); i++) {
    Pair mapObjectPair = mapObjects[i];
    if (!mapObjectPair.first) continue;

    if (alreadyAdded.contains(mapObjectPair.first)) continue;
    alreadyAdded.insert(mapObjectPair.first);

    // Tell the plugins to fill the map object menu
    foreach(PluginBase* plugin, updraft->pluginManager->getAllPlugins()) {
      plugin->fillContextMenu(mapObjectPair.first, moMenu);
    }

    // If there was at least one action added, create a submenu
    if (!moMenu->getQMenu()->isEmpty()) {
      QMenu* submenu = moMenu->giveQMenu();
      QAction* action = new QAction(mapObjectPair.first->name, NULL);
      action->setMenu(submenu);
      submenuActions.push_back(action);

      // Use only light clear here, so that the menu's actions are not deleted
      moMenu->lightClear();
    }
  }

  // If there were no actions at all, don't do anything
  if (submenuActions.empty()) return;

  // Insert the submenus into the top-level menu
  foreach(QAction* action, submenuActions) {
    moMenu->appendAction(action);

    // Remember the action for deletion later
    ownedActions.push_back(action);
  }

  // Show the menu
  QWidget* mapWidget = updraft->mainWindow->getMapWidget();
  int mh = mapWidget->height();

  // The coordinates have to be Y-inverted and mapped to screen
  moMenu->getQMenu()->exec(mapWidget->mapToGlobal(QPoint(mX, mh - mY)));
}

MapObject* PickHandler::getMapObject(osg::Node* node) {
  return updraft->sceneManager->getNodeMapObject(node);
}

bool PickHandler::eventsCloseEnough(float x1, float y1, float x2, float y2) {
  float dx = x1 - x2;
  float dy = y1 - y2;
  float tolerance = mouseEventTolerance->get().toFloat();

  if (dy <= tolerance && dy >= -tolerance &&
      dx <= tolerance && dx >= -tolerance) {
    return true;
  }

  return false;
}

QVector<Pair> PickHandler::getIntersectedMapObjects(
  const double x,
  const double y,
  osgViewer::Viewer* viewer ) {
  // TODO(bohdan): settings? increase in interations?
  // (to improve tolerance without crippling accuracy)
  double w(.005), h(.005);

  osgUtil::PolytopeIntersector* picker =
    new osgUtil::PolytopeIntersector(
      osgUtil::Intersector::PROJECTION, x-w, y-h, x+w, y+h);

  osgUtil::IntersectionVisitor iv(picker);
  viewer->getCamera()->accept(iv);

  osgUtil::PolytopeIntersector::Intersections& intersections =
    picker->getIntersections();

  /*qDebug(":::::::::::::::::::::::::");
  std::set<osgUtil::PolytopeIntersector::Intersection>::iterator it;
  int i = 0;
  for (it = intersections.begin(); it != intersections.end(); ++it) {
    qDebug() << "Intersection " << i << ":";
    qDebug() << "d: " << it->distance << ", max_d: " << it->maxDistance <<
      ", local_ip: " <<
      it->localIntersectionPoint.x() << ":" <<
      it->localIntersectionPoint.y() << ":" <<
      it->localIntersectionPoint.z() <<
      ", numIntersectionPoints: " << it->numIntersectionPoints;
    osg::Vec4f vec(
      it->localIntersectionPoint.x(),
      it->localIntersectionPoint.y(),
      it->localIntersectionPoint.z(),
      1.0);
    osg::Matrixf mat = (*it->matrix.get());
    osg::Matrixf mat_inv = osg::Matrixf::inverse(mat);
    osg::Matrixf cam_mat = viewer->getCamera()->getProjectionMatrix();
    cam_mat = osg::Matrixf::inverse(cam_mat);
    osg::Vec4f w_lip = vec * mat;
    osg::Vec3f w_lip2 = cam_mat * it->localIntersectionPoint;
    qDebug() << "Transformed localIntersectionPoint: " <<
      w_lip.x() << ":" <<
      w_lip.y() << ":" <<
      w_lip.z() << ":::" <<
      w_lip.w();
    qDebug() << "Transformed localIntersectionPoint 2: " <<
      w_lip2.x() << ":" <<
      w_lip2.y() << ":" <<
      w_lip2.z();
      for (int p = 0; p < it->numIntersectionPoints; ++p) {
        qDebug() << "  intersectionPoint[" << p << "]: " <<
          it->intersectionPoints[p].x() << ":" <<
          it->intersectionPoints[p].y() << ":" <<
          it->intersectionPoints[p].z();
      }
    ++i;
  }
  qDebug(":::::::::::::::::::::::::");*/

  QVector<Pair> mapObjects;

  std::set<osgUtil::PolytopeIntersector::Intersection>::iterator it;
  for (it = intersections.begin(); it != intersections.end(); ++it) {
    const osg::NodePath& nodePath = it->nodePath;
    unsigned int idx = nodePath.size();
    while (idx--) {
      MapObject* mapObject = getMapObject(nodePath[idx]);
      if (mapObject != NULL) {
        // Transform the intersection coordinates to world coordinates
        osg::Vec4f vec(
          it->intersectionPoints[0].x(),
          it->intersectionPoints[0].y(),
          it->intersectionPoints[0].z(),
          1.0);
        osg::Matrixf mat = (*it->matrix.get());
        osg::Vec4f w_vec = vec * mat;
        osg::Vec3f w_vec3f(
            w_vec.x(),
            w_vec.y(),
            w_vec.z());

        mapObjects.append(Pair(mapObject, w_vec3f));
        qDebug("Distance of intersection point:");
        qDebug() << it->distance;
      }
    }
  }

  return mapObjects;
}

}  // End namespace Core
}  // End namespace Updraft

