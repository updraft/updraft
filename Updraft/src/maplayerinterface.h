#ifndef UPDRAFT_SRC_MAPLAYERINTERFACE_H_
#define UPDRAFT_SRC_MAPLAYERINTERFACE_H_

namespace osg {
  class Node;
}

namespace osgEarth {
  class ImageLayer;
  class ElevationLayer;
  class ModelLayer;
}

class QObject;

namespace Updraft {

/// Type of the map layer.
enum MapLayerType {
  OSG_NODE_LAYER,
  IMAGE_LAYER,
  ELEVATION_LAYER,
  MODEL_LAYER
};

/// Map layer
union Layer {
  osg::Node* osgNode;
  osgEarth::ImageLayer* imageLayer;
  osgEarth::ElevationLayer* elevationLayer;
  osgEarth::ModelLayer* modelLayer;
};

/// An interface to one map layer that is used by plugins.
class MapLayerInterface {
 public:
  virtual ~MapLayerInterface() {}

  /// Connects to a signal displayed(bool) -- map layer is
  /// selected / diselected in the left panel.
  /// \see QObject::connect()
  virtual void connectSignalDisplayed(const QObject* receiver,
    const char *method) = 0;

  /// Emits the displayed(bool) signal.
  virtual void emitDisplayed(bool value) = 0;

  virtual Layer getLayer() = 0;
  virtual void setLayer(Layer l) = 0;

  virtual MapLayerType getType() = 0;
  virtual void setType(MapLayerType type) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MAPLAYERINTERFACE_H_
