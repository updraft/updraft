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
}

QVector<osgEarth::ImageLayer*> MapManager::getImageLayers() {
  QVector<osgEarth::ImageLayer*> imageLayers;
  osgEarth::ImageLayerVector outImageLayers;
  map->getImageLayers(outImageLayers);
  for (uint i = 0; i < outImageLayers.size(); i++) {
    imageLayers.append(outImageLayers[i]);
  }
  return imageLayers;
}

QVector<osgEarth::ElevationLayer*> MapManager::getElevationLayers() {
  QVector<osgEarth::ElevationLayer*> elevationLayers;
  osgEarth::ElevationLayerVector outElevationLayers;
  map->getElevationLayers(outElevationLayers);
  for (uint i = 0; i < outElevationLayers.size(); i++) {
    elevationLayers.append(outElevationLayers[i]);
  }
  return elevationLayers;
}

QVector<osgEarth::ModelLayer*> MapManager::getModelLayers() {
  QVector<osgEarth::ModelLayer*> modelLayers;
  osgEarth::ModelLayerVector outModelLayers;
  map->getModelLayers(outModelLayers);
  for (uint i = 0; i < outModelLayers.size(); i++) {
    modelLayers.append(outModelLayers[i]);
  }
  return modelLayers;
}

QVector<MapLayerInterface*> MapManager::getMapLayers() {
  return mapLayers;
}

void MapManager::fillMapLayerGroup(MapLayerGroupInterface* group) {
  mapLayerGroup = group;

  osgEarth::ImageLayerVector outImageLayers;
  map->getImageLayers(outImageLayers);
  for (uint i = 0; i < outImageLayers.size(); i++) {
    QString name = QString::fromStdString(outImageLayers[i]->getName());
    MapLayerInterface* layer =
      mapLayerGroup->insertExistingMapLayer(outImageLayers[i], name);
    layer->connectDisplayedToVisibility();
    mapLayers.append(layer);
  }

  osgEarth::ElevationLayerVector outElevationLayers;
  map->getElevationLayers(outElevationLayers);
  for (uint i = 0; i < outElevationLayers.size(); i++) {
    QString name = QString::fromStdString(outElevationLayers[i]->getName());
    MapLayerInterface* layer =
      mapLayerGroup->insertExistingMapLayer
      (outElevationLayers[i], name);
    layer->connectDisplayedToVisibility();
    mapLayers.append(layer);
  }

  osgEarth::ModelLayerVector outModelLayers;
  map->getModelLayers(outModelLayers);
  for (uint i = 0; i < outModelLayers.size(); i++) {
    QString name = QString::fromStdString(outModelLayers[i]->getName());
    MapLayerInterface* layer =
      mapLayerGroup->insertExistingMapLayer(outModelLayers[i], name);
    layer->connectDisplayedToVisibility();
    mapLayers.append(layer);
  }
}

osgEarth::MapNode* MapManager::getMapNode() {
  return mapNode;
}
}
}
