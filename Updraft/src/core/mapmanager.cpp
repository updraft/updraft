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
  // TODO(Maria): parse image layers from the map built from earth file and set them in the structure.
}

osgEarth::MapNode* MapManager::getMapNode() {
  return mapNode;
}

osgEarth::MapNode* MapManager::createMap(QString earthFile) {
  // delete the old map
  if (mapNode != NULL) {
    // TODO(Maria): is this correct way of deleting the whole map?
    // delete (mapNode);
    mapNode = NULL;
    map = NULL;
  }
  osg::Node* loadedMap = osgDB::readNodeFile(earthFile.toStdString());
  this->mapNode = osgEarth::MapNode::findMapNode(loadedMap);
  this->map = mapNode->getMap();

    // set image layers
  // TODO(Maria): parse image layers from the map build from the earth file.

  return mapNode;
}

}
}
