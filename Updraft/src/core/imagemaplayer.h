#ifndef UPDRAFT_SRC_CORE_IMAGEMAPLAYER_H_
#define UPDRAFT_SRC_CORE_IMAGEMAPLAYER_H_

#include "maplayer.h"

#include <osg/ref_ptr>
#include <osgEarth/ImageLayer>

namespace Updraft {
namespace Core {

class ImageMapLayer: public MapLayer {
  Q_OBJECT

 public:
  explicit ImageMapLayer(osgEarth::ImageLayer* layer);

  void setVisible(bool value);
  bool isVisible();

 private:
  osg::ref_ptr<osgEarth::ImageLayer> layer;

  void addToScene(MapLayerGroup *group);
  void removeFromScene(MapLayerGroup *group);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_IMAGEMAPLAYER_H_

