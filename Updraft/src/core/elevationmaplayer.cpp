#include "elevationmaplayer.h"

#include <osgEarth/MapNode>

#include "ui/maplayergroup.h"

namespace Updraft {
namespace Core {

ElevationMapLayer::ElevationMapLayer(osgEarth::ElevationLayer* layer)
  : layer(layer) {}

void ElevationMapLayer::setVisible(bool value) {
  layer->setEnabled(value);
}

bool ElevationMapLayer::isVisible() {
  return layer->getEnabled();
}

void ElevationMapLayer::addToScene(MapLayerGroup *group) {
  group->getMapNode()->getMap()->addElevationLayer(layer);
}

void ElevationMapLayer::removeFromScene(MapLayerGroup *group) {
  group->getMapNode()->getMap()->removeElevationLayer(layer);
}

}  // End namespace Core
}  // End namespace Updraft

