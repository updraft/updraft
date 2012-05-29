#include "modelmaplayer.h"

#include <osgEarth/MapNode>

#include "ui/maplayergroup.h"

namespace Updraft {
namespace Core {

ModelMapLayer::ModelMapLayer(osgEarth::ModelLayer* layer)
  : layer(layer) {}

void ModelMapLayer::setVisible(bool value) {
  layer->setEnabled(value);
}

bool ModelMapLayer::isVisible() {
  return layer->getEnabled();
}

void ModelMapLayer::addToScene(MapLayerGroup *group) {
  group->getMapNode()->getMap()->addModelLayer(layer);
}

void ModelMapLayer::removeFromScene(MapLayerGroup *group) {
  group->getMapNode()->getMap()->removeModelLayer(layer);
}

}  // End namespace Core
}  // End namespace Updraft

