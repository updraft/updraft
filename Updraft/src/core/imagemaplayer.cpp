#include "imagemaplayer.h"

#include <osgEarth/MapNode>

#include "ui/maplayergroup.h"

namespace Updraft {
namespace Core {

ImageMapLayer::ImageMapLayer(osgEarth::ImageLayer* layer)
  : layer(layer) {}

void ImageMapLayer::setVisible(bool value) {
  layer->setEnabled(value);
}

bool ImageMapLayer::isVisible() {
  return layer->getEnabled();
}

void ImageMapLayer::addToScene(MapLayerGroup *group) {
  group->getMapNode()->getMap()->addImageLayer(layer);
}

void ImageMapLayer::removeFromScene(MapLayerGroup *group) {
  group->getMapNode()->getMap()->removeImageLayer(layer);
}

}  // End namespace Core
}  // End namespace Updraft

