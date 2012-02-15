#ifndef UPDRAFT_SRC_CORE_MAPLAYER_H_
#define UPDRAFT_SRC_CORE_MAPLAYER_H_

#include <QObject>
#include "../maplayerinterface.h"

namespace Updraft {
namespace Core {

/// A class that represents one logical layer in map.
/// Map layer in osgEarth can be of different type, and this
/// class encapsulates all of the types, and creates common
/// interface.
/// The type of the map layer determines which value of the
/// union Layer is valid.
class MapLayer : public QObject, public MapLayerInterface {
  Q_OBJECT

 public:
  /// Creates an empty map layer.
  MapLayer();

  /// Creates a MapLayer with given values.
  MapLayer(MapLayerType type, Layer layer);

  /// Constructs a MapLayer and sets its type to OSG_NODE_LAYER.
  explicit MapLayer(osg::Node* node);

  /// Constructs a MapLayer and sets its type to IMAGE_LAYER.
  explicit MapLayer(osgEarth::ImageLayer* imageLayer);

  /// Constructs a MapLayer and sets its type to ELEVATION_LAYER.
  explicit MapLayer(osgEarth::ElevationLayer* elevationLayer);

  /// Constructs a MapLayer and sets its type to MODEL_LAYER.
  explicit MapLayer(osgEarth::ModelLayer* modelLayer);

  ~MapLayer();

  void connectSignalDisplayed(const QObject* receiver,
    const char *method);
  void connectSlotSetVisibility(const QObject* sender,
    const char *method);
  void connectDisplayedToVisibility();
  void emitDisplayed(bool value);

  void setVisible(bool value);

  MapLayerType getType();
  Layer getLayer();
  void setLayer(Layer l);
  void setType(MapLayerType type);

 private slots:
  void setVisibility(bool value);

 signals:
  void displayed(bool value, MapLayerInterface* sender);

 private:
  /// Determines which value of the union layer is valid.
  MapLayerType layerType;

  /// The pointer to the appropriate map layer structure.
  Layer layer;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPLAYER_H_
