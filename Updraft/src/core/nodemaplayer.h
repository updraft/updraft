#ifndef UPDRAFT_SRC_CORE_NODEMAPLAYER_H_
#define UPDRAFT_SRC_CORE_NODEMAPLAYER_H_

#include <osg/Node>
#include <osg/ref_ptr>

#include "maplayer.h"


namespace Updraft {
namespace Core {

/// Map layer that contains an osg::Node with geometry.
class NodeMapLayer: public MapLayer {
  Q_OBJECT

 public:
  NodeMapLayer(const QString &title, osg::Node* node);
  ~NodeMapLayer();

  /// Displays/hides the map layer by setting/hiding the node.
  /// \param value Whether the node should be visible.
  void setVisibility(bool value);

  /// Tells whether the node is currently visible.
  /// \return Whether the node is currently visible.
  bool isVisible();

 private:
  /// The node that contains the map layer geometry.
  osg::ref_ptr<osg::Node> node;

  void addToScene(MapLayerGroup *group);
  void removeFromScene(MapLayerGroup *group);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_NODEMAPLAYER_H_

