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

MapLayer::~MapLayer() {
}

void MapLayer::connectSignalDisplayed(const QObject *receiver,
  const char *method) {
  connect(this, SIGNAL(displayed(bool)), receiver, method);
}

void MapLayer::emitDisplayed(bool value) {
  emit displayed(value);
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
    case MapLayerType::OSG_NODE_LAYER: {
      if (value == true) {
        layer.osgNode->setNodeMask(0xffffffff);
      } else {
        layer.osgNode->setNodeMask(0x0);
      }
      break;
    }
    case MapLayerType::IMAGE_LAYER: {
      layer.imageLayer->setEnabled(value);
      break;
    }
    case MapLayerType::ELEVATION_LAYER: {
      layer.elevationLayer->setEnabled(value);
      break;
    }
    case MapLayerType::MODEL_LAYER: {
      layer.modelLayer->setEnabled(value);
      break;
    }
  }
}

}  // End namespace Core
}  // End namespace Updraft
