#include "mapmanager.h"
#include <osgDB/ReadFile>
#include <osgEarthUtil/ElevationManager>
#include <QDebug>
#include <string>

namespace Updraft {
namespace Core {

MapManager::MapManager() {
  this->mapNode = NULL;
  this->map = NULL;
}

MapManager::MapManager(QString earthFile) {
  osg::Node* loadedMap = osgDB::readNodeFile(earthFile.toStdString());
  if (loadedMap != NULL) {
    this->mapNode = osgEarth::MapNode::findMapNode(loadedMap);
    this->map = mapNode->getMap();
  } else {
    this->map = new osgEarth::Map();
    this->mapNode = new osgEarth::MapNode(this->map);
  }

  // parse layers from the map built from earth file
  // and set them in the structure.
  osgEarth::ImageLayerVector outImageLayers;
  map->getImageLayers(outImageLayers);
  for (uint i = 0; i < outImageLayers.size(); i++) {
    imageLayers.append(outImageLayers[i]);
  }

  osgEarth::ElevationLayerVector outElevationLayers;
  map->getElevationLayers(outElevationLayers);
  osgEarth::Util::ElevationManager* elevationManager = new
    osgEarth::Util::ElevationManager(map);
  double height;
  double resolution;
  elevationManager->getElevation(27.996, 86.927, 0, NULL, height, resolution);
  qDebug() << height;
  for (uint i = 0; i < outElevationLayers.size(); i++) {
    elevationLayers.append(outElevationLayers[i]);
  }

  osgEarth::ModelLayerVector outModelLayers;
  map->getModelLayers(outModelLayers);
  for (uint i = 0; i < outModelLayers.size(); i++) {
    modelLayers.append(outModelLayers[i]);
  }
}

osgEarth::MapNode* MapManager::getMapNode() {
  return mapNode;
}

osgEarth::MapNode* MapManager::createMap(QString earthFile) {
  // delete the old map
  if (mapNode != NULL) {
    // TODO(Maria): is this correct way of deleting the whole map?
    // delete (mapNode);
    // mapNode = NULL;
    // map = NULL;
  }
  osg::Node* loadedMap = osgDB::readNodeFile(earthFile.toStdString());
  // this->mapNode = osgEarth::MapNode::findMapNode(loadedMap);
  osgEarth::MapNode* mapNode2 = osgEarth::MapNode::findMapNode(loadedMap);
  // this->map = mapNode->getMap();

    // set image layers
  // TODO(Maria): parse image layers from the map build from the earth file.

  return mapNode2;
}
}
}
