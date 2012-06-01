#include "nodemaplayer.h"

#include <osg/Group>

#include "ui/maplayergroup.h"

namespace Updraft {
namespace Core {

NodeMapLayer::NodeMapLayer(osg::Node* node)
  : node(node) {}

void NodeMapLayer::setVisible(bool value) {
  node->setNodeMask(value ? 0xffffffff : 0x0);
  emit displayed(value, this);
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

