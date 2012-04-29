#include "maplayer.h"
#include <osg/Node>
#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/ModelLayer>

namespace Updraft {
namespace Core {

MapLayer::MapLayer(MapLayerType t, Layer l) {
  layerType = t;
  layer = l;
}

MapLayer::MapLayer() {
}

MapLayer::MapLayer(osg::Node* node) {
  layerType = OSG_NODE_LAYER;
  layer.osgNode = node;
}

MapLayer::MapLayer(osgEarth::ImageLayer* imageLayer) {
  layerType = IMAGE_LAYER;
  layer.imageLayer = imageLayer;
}

MapLayer::MapLayer(osgEarth::ElevationLayer* elevationLayer) {
  layerType = ELEVATION_LAYER;
  layer.elevationLayer = elevationLayer;
}

MapLayer::MapLayer(osgEarth::ModelLayer* modelLayer) {
  layerType = MODEL_LAYER;
  layer.modelLayer = modelLayer;
}

MapLayer::~MapLayer() {
}

void MapLayer::connectSignalDisplayed(const QObject *receiver,
  const char *method) {
  connect(this, SIGNAL(displayed(bool, MapLayerInterface*)), receiver, method);
}

void MapLayer::connectSlotSetVisibility(const QObject *sender,
  const char* method) {
  connect(sender, method, this, SLOT(setVisibility(bool)));
}

void MapLayer::connectDisplayedToVisibility() {
  connect(this, SIGNAL(displayed(bool, MapLayerInterface*)),
    this, SLOT(setVisibility(bool)));
}

void MapLayer::emitDisplayed(bool value) {
  emit displayed(value, this);
}

void MapLayer::setVisibility(bool value) {
  setVisible(value);
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

bool MapLayer::isVisible() {
  switch (layerType) {
    case OSG_NODE_LAYER: {
      if (layer.osgNode->getNodeMask() == 0x0) {
        return false;
      } else {
        return true;
      }
      break;
    }
    case IMAGE_LAYER: {
      return layer.imageLayer->getEnabled();
      break;
    }
    case ELEVATION_LAYER: {
      return layer.elevationLayer->getEnabled();
      break;
    }
    case MODEL_LAYER: {
      return layer.modelLayer->getEnabled();
      break;
    }
  }
  return false;
}

}  // End namespace Core
}  // End namespace Updraft
