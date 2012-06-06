#include "mapmanager.h"
#include <osgDB/ReadFile>
#include <osgEarthUtil/ElevationManager>
#include <osgEarth/Cache>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <QDebug>
#include <string>
#include "updraft.h"

namespace Updraft {
namespace Core {

MapManager::MapManager() {
  this->map = new osgEarth::Map();
  QString name = tr("Empty globe");
  this->map->setName(name.toStdString());
  this->mapNode = new osgEarth::MapNode(this->map);
  is3d = true;

  this->manipulator = new MapManipulator();
  manipulator->setNode(mapNode);
}

MapManager::MapManager(QString earthFile, QString mapName_,
  bool hasSkyNode_, osgViewer::Viewer* viewer_) {
  earthFileName = earthFile;
  this->mapName = mapName_;
  hasSkyNode = hasSkyNode_;
  viewer = viewer_;
  mapNode = NULL;
  createMap();
}

void MapManager::createMap() {
  if (mapNode != NULL) destroyMap();

  QDir dataDir = updraft->getDataDirectory();
  QString earthPath = dataDir.absoluteFilePath(earthFileName);

  osg::Node* loadedMap = osgDB::readNodeFile(earthPath.toStdString());
  if (loadedMap != NULL) {
    this->mapNode = osgEarth::MapNode::findMapNode(loadedMap);
    this->map = mapNode->getMap();
    this->map->setName(mapName.toStdString());
  } else {
    this->map = new osgEarth::Map();
    QString name = tr("Empty globe");
    this->map->setName(mapName.toStdString());
    this->mapNode = new osgEarth::MapNode(this->map);
  }
    // initialize the manipulator
  setManipulator(new MapManipulator());

  if (hasSkyNode) {
    skyNode = new osgEarth::Util::SkyNode(map);
    skyNode->attach(viewer);
    skyNode->setSunPosition(14.42046, 50.087811);
    skyNode->setAmbientBrightness(0.5);
  }
}

void MapManager::destroyMap() {
  mapNode = NULL;
  skyNode = NULL;
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

void MapManager::updateCameraProjection() {
  if (!hasSkyNode) {
    getManipulator()->updateCameraProjection();
  }
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

void MapManager::attach(osg::Group* scene) {
  scene->addChild(mapNode);
  if (hasSkyNode) {
    scene->addChild(skyNode);
  }
}

void MapManager::detach(osg::Group* scene) {
  scene->removeChild(this->getMapNode());
  if (hasSkyNode) {
    scene->removeChild(skyNode);
  }
}

QVector<MapLayerInterface*> MapManager::getMapLayers() {
  return mapLayers;
}

osgEarth::MapNode* MapManager::getMapNode() {
  return mapNode;
}

osgEarth::Map* MapManager::getMap() {
  return map;
}

MapManipulator* MapManager::getManipulator() {
  return manipulator;
}

void MapManager::setManipulator(MapManipulator* newManipulator) {
  newManipulator->setNode(mapNode);
  manipulator = newManipulator;
}

MapObject* MapManager::getMapObject() {
  return &mapObject;
}

QString MapManager::getName() {
  return QString::fromStdString(map->getName());
}

bool MapManager::hasElevation() {
  osgEarth::ElevationLayerVector outElevationLayers;
  map->getElevationLayers(outElevationLayers);
  if (outElevationLayers.size() > 0) {
    return true;
  } else {
    return false;
  }
}

}  // End namespace Core
}  // End namespace Updraft
