#ifndef UPDRAFT_SRC_CORE_NODEMAPLAYER_H_
#define UPDRAFT_SRC_CORE_NODEMAPLAYER_H_

#include <osg/Node>
#include <osg/ref_ptr>

#include "maplayer.h"


namespace Updraft {
namespace Core {

class NodeMapLayer: public MapLayer {
  Q_OBJECT

 public:
  NodeMapLayer(const QString &title, osg::Node* node);
  ~NodeMapLayer();

  void setVisibility(bool value);
  bool isVisible();

 private:
  osg::ref_ptr<osg::Node> node;

  void addToScene(MapLayerGroup *group);
  void removeFromScene(MapLayerGroup *group);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_NODEMAPLAYER_H_

