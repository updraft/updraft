#include "mapmanager.h"
#include <osgDB/ReadFile>

namespace Updraft {
namespace Core {

MapManager::MapManager() {
  this->mapNode = NULL;
  this->map = NULL;
}

MapManager::MapManager(QString earthFile) {
  osg::Node* loadedMap = osgDB::readNodeFile(earthFile.toStdString());
  this->mapNode = osgEarth::MapNode::findMapNode(loadedMap);
  this->map = mapNode->getMap();

    // set image layers
  // TODO
}

osgEarth::MapNode* MapManager::getMapNode() {
  return mapNode;
}

osgEarth::MapNode* MapManager::createMap(QString earthFile) {
  // delete the old map
  if (mapNode != NULL) {
    // TODO: is this correct way of deleting the whole map?
    //delete (mapNode);
    mapNode = NULL;
    map = NULL;
  }

  osg::Node* loadedMap = osgDB::readNodeFile(earthFile.toStdString());
  this->mapNode = osgEarth::MapNode::findMapNode(loadedMap);
  this->map = mapNode->getMap();

    // set image layers
  // TODO

  return mapNode;
}

}
}