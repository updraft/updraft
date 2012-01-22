#include "maplayer.h"
#include <osg/Node>
#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/ModelLayer>

namespace Updraft {
namespace Core {

MapLayer::MapLayer(int i, MapLayerType t, Layer l) {
  id = i;
  layerType = t;
  layer = l;
}

MapLayer::MapLayer(int i) {
  id = i;
}

MapLayer::MapLayer(int i, osg::Node* node) {
  id = i;
  layerType = OSG_NODE_LAYER;
  layer.osgNode = node;
}

MapLayer::MapLayer(int i, osgEarth::ImageLayer* imageLayer) {
  id = i;
  layerType = IMAGE_LAYER;
  layer.imageLayer = imageLayer;
}

MapLayer::MapLayer(int i, osgEarth::ElevationLayer* elevationLayer) {
  id = i;
  layerType = ELEVATION_LAYER;
  layer.elevationLayer = elevationLayer;
}

MapLayer::MapLayer(int i, osgEarth::ModelLayer* modelLayer) {
  id = i;
  layerType = MODEL_LAYER;
  layer.modelLayer = modelLayer;
}

MapLayer::~MapLayer() {
}

void MapLayer::connectSignalDisplayed(const QObject *receiver,
  const char *method) {
  connect(this, SIGNAL(displayed(int, bool)), receiver, method);
}

void MapLayer::emitDisplayed(bool value) {
  emit displayed(id, value);
}

MapLayerType MapLayer::getType() {
  return layerType;
}

Layer MapLayer::getLayer() {
  return layer;
}

void MapLayer::setType(MapLayerType type) {
  layerType = type;
}

void MapLayer::setLayer(Layer l) {
  layer = l;
}

void MapLayer::setVisible(bool value) {
  switch (layerType) {
    case OSG_NODE_LAYER: {
      if (value == true) {
        layer.osgNode->setNodeMask(0xffffffff);
      } else {
        layer.osgNode->setNodeMask(0x0);
      }
      break;
    }
    case IMAGE_LAYER: {
      layer.imageLayer->setEnabled(value);
      break;
    }
    case ELEVATION_LAYER: {
      layer.elevationLayer->setEnabled(value);
      break;
    }
    case MODEL_LAYER: {
      layer.modelLayer->setEnabled(value);
      break;
    }
  }
}

}  // End namespace Core
}  // End namespace Updraft
