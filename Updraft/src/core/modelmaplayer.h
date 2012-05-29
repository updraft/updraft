#ifndef UPDRAFT_SRC_CORE_MODELMAPLAYER_H_
#define UPDRAFT_SRC_CORE_MODELMAPLAYER_H_

#include <osgEarth/ModelLayer>
#include <osg/ref_ptr>

#include "maplayer.h"


namespace Updraft {
namespace Core {

class ModelMapLayer: public MapLayer {
  Q_OBJECT

 public:
  explicit ModelMapLayer(osgEarth::ModelLayer* layer);

  void setVisible(bool value);
  bool isVisible();

 private:
  osg::ref_ptr<osgEarth::ModelLayer> layer;

  void addToScene(MapLayerGroup *group);
  void removeFromScene(MapLayerGroup *group);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MODELMAPLAYER_H_

