#include "pickhandler.h"

#include <QSet>
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
        QVector<MapObject*> mapObjects = getIntersectedMapObjects(
          ea.getXnormalized(), ea.getYnormalized(), viewer);

        if (mapObjects.empty())
          return false;

	int btn = ea.getButton();
	if (btn == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
          raiseLeftClick(mapObjects);
	} else if (btn == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
	  raiseRightClick(mapObjects);
	}
        return true;
      }
      return false;
    default:
      return false;
  }
}

void PickHandler::raiseLeftClick(QVector<MapObject*> mapObjects) {
  qDebug("--- Left click event ---");

  Menu* moMenu = updraft->mainWindow->getSystemMenu(MENU_MAPOBJECT);
  moMenu->clear();

  EventInfo eventInfo(LEFT_CLICK);

  // Create the PickActions and prevent double adding of objects
  QSet<MapObject*> alreadyAdded;
  foreach(MapObject* mapObject, mapObjects) {
    if (!mapObject) continue;

    if (alreadyAdded.contains(mapObject)) continue;
    alreadyAdded.insert(mapObject);

    // Insert the plugins into PickAction only if they want to handle the event
    PickAction* action = new PickAction(mapObject, &eventInfo);
    foreach(PluginBase* plugin, updraft->pluginManager->getAllPlugins()) {
      if (plugin->wantsToHandleClick(mapObject)) {
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
  if (moMenu->getQMenu()->actions().size() > 1) {
    QWidget* mapWidget = updraft->mainWindow->getMapWidget();
    int mh = mapWidget->height();

    // The coordinates have to be Y-inverted and mapped to screen
    moMenu->getQMenu()->exec(mapWidget->mapToGlobal(QPoint(mX, mh - mY)));
  } else {  // If there was only one inserted action, trigger it
    moMenu->getQMenu()->actions().first()->trigger();
  }
}

void PickHandler::raiseRightClick(QVector<MapObject*> mapObjects) {
  qDebug("--- Right click event ---");

  QMenu* topMenu = new QMenu(updraft->mainWindow);

  // Create the submenu where plugins will push their actions
  QMenu* qMenu = new QMenu(updraft->mainWindow);
  Menu objectMenu(qMenu);

  // Prevent double adding of objects
  QSet<MapObject*> alreadyAdded;
  foreach (MapObject* mapObject, mapObjects) {
    if (!mapObject) continue;

    if (alreadyAdded.contains(mapObject)) continue;
    alreadyAdded.insert(mapObject);

    // Let each plugin add items to the submenu
    foreach (PluginBase* plugin, updraft->pluginManager->getAllPlugins()) {
      plugin->fillContextMenu(mapObject, &objectMenu);
    }
   
    // If the menu is not empty, add its actions to the top-level menu
    if (!qMenu->isEmpty()) {
      if (!topMenu->isEmpty()) {
        topMenu->addSeparator();
      }

      // Create the MapObject name action
      QAction* name = new QAction(mapObject->name, NULL);
      name->setEnabled(false);  // We don't want the name to be clickable
      topMenu->addAction(name);

      foreach (QAction* action, qMenu->actions()) {
	topMenu->addAction(action);
      }
    }

    // Clear the helper submenu
    qMenu->clear();
  }

  // Show the menu
  QWidget* mapWidget = updraft->mainWindow->getMapWidget();
  int mh = mapWidget->height();
  topMenu->exec(mapWidget->mapToGlobal(QPoint(mX, mh - mY)));

  delete qMenu;
  delete topMenu;
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

