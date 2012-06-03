#include "nodemaplayer.h"

#include <osg/Group>

#include "ui/maplayergroup.h"

namespace Updraft {
namespace Core {

NodeMapLayer::NodeMapLayer(const QString &title, osg::Node* node)
  : MapLayer(title), node(node) {}

NodeMapLayer::~NodeMapLayer() {
  // The following code must be in the same class that defines
  // removeFromScene instead of in the common ancestor,
  // because of how virtual calls are disabled in destructors.
  // Pure magic :-)
  if (parent) {
    parent->removeMapLayer(this);
  }
}

void NodeMapLayer::setVisibility(bool value) {
  node->setNodeMask(value ? 0xffffffff : 0x0);
}

bool NodeMapLayer::isVisible() {
  return node->getNodeMask() != 0x0;
}

void NodeMapLayer::addToScene(MapLayerGroup *group) {
  group->getNodeGroup()->addChild(node);
}

void NodeMapLayer::removeFromScene(MapLayerGroup *group) {
  group->getNodeGroup()->removeChild(node);
}

}  // End namespace Core
}  // End namespace Updraft

