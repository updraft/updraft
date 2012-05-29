#ifndef UPDRAFT_SRC_CORE_ELEVATIONMAPLAYER_H_
#define UPDRAFT_SRC_CORE_ELEVATIONMAPLAYER_H_

#include <osgEarth/ElevationLayer>
#include <osg/ref_ptr>

#include "maplayer.h"


namespace Updraft {
namespace Core {

class ElevationMapLayer: public MapLayer {
  Q_OBJECT

 public:
  explicit ElevationMapLayer(osgEarth::ElevationLayer* layer);

  void setVisible(bool value);
  bool isVisible();

 private:
  osg::ref_ptr<osgEarth::ElevationLayer> layer;

  void addToScene(MapLayerGroup *group);
  void removeFromScene(MapLayerGroup *group);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_ELEVATIONMAPLAYER_H_

