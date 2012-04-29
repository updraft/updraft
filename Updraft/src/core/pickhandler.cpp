#include "pickhandler.h"

#include <QSet>
#include <QPair>
#include "scenemanager.h"
#include "updraft.h"
#include "pickaction.h"
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

    // Insert the plugins into PickAction only if they want to handle the event
    PickAction* action = new PickAction(mapObjectPair.first,
      new EventInfo(LEFT_CLICK, mapObjectPair.second));
    qDebug() << mapObjectPair.second.x() << mapObjectPair.second.y() <<
      mapObjectPair.second.z();
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
  } else {  // If there was only one inserted action, trigger it
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

QVector<Pair> PickHandler::getIntersectedMapObjects(
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

  QVector<Pair> mapObjects;

  std::set<osgUtil::PolytopeIntersector::Intersection>::iterator it;
  for (it = intersections.begin(); it != intersections.end(); ++it) {
    const osg::NodePath& nodePath = it->nodePath;
    unsigned int idx = nodePath.size();
    while (idx--) {
      MapObject* mapObject = getMapObject(nodePath[idx]);
      if (mapObject != NULL) {
        mapObjects.append(Pair(mapObject, it->intersectionPoints[0]));
      }
    }
  }

  return mapObjects;
}

}  // End namespace Core
}  // End namespace Updraft

