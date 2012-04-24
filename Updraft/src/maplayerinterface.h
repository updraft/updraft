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

/// A structure for storing a pointer to particular map layer.
/// As it is a union, there is always exactly one value valid.
union Layer {
  osg::Node* osgNode;
  osgEarth::ImageLayer* imageLayer;
  osgEarth::ElevationLayer* elevationLayer;
  osgEarth::ModelLayer* modelLayer;
};

/// An interface to a map layer that is used by plugins.
/// More about map layers \see class MapLayer.
class MapLayerInterface {
 public:
  virtual ~MapLayerInterface() {}

  /// Connects to a signal displayed(MapLayerInterface*, bool):
  /// map layer is selected / diselected in the left panel.
  /// \see QObject::connect()
  virtual void connectSignalDisplayed(const QObject* receiver,
    const char *method) = 0;

  /// Connects to a slot setVisibility(bool) -- map layer sets
  /// the visibility to the layer in the scene graph.
  /// \see QObject::connect()
  virtual void connectSlotSetVisibility(const QObject* sender,
    const char *method) = 0;

  /// Automatically connects the signal whether the map is
  /// selected / deselected in the menu to the
  /// setVisibility slot, so that the map enables / disabled it's
  /// visibility.
  virtual void connectDisplayedToVisibility() = 0;

  /// Emits the displayed(bool) signal.
  virtual void emitDisplayed(bool value) = 0;

  /// Sets visibility to the layer.
  virtual void setVisible(bool value) = 0;

  virtual bool isVisible() = 0;

  virtual Layer getLayer() = 0;
  virtual void setLayer(Layer l) = 0;

  virtual MapLayerType getType() = 0;
  virtual void setType(MapLayerType type) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MAPLAYERINTERFACE_H_
