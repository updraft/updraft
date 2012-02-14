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
  MapLayer(MapLayerGroup* parentMapLayerGroup, QTreeWidgetItem* checkbox);

  /// Creates a MapLayer with given values.
  MapLayer(MapLayerType type, Layer layer,
    MapLayerGroup* parentMapLayerGroup, QTreeWidgetItem* checkbox);

  /// Constructs a MapLayer and sets its type to OSG_NODE_LAYER.
  MapLayer(osg::Node* node,
    MapLayerGroup* parentMapLayerGroup, QTreeWidgetItem* checkbox);

  /// Constructs a MapLayer and sets its type to IMAGE_LAYER.
  MapLayer(osgEarth::ImageLayer* imageLayer,
    MapLayerGroup* parentMapLayerGroup, QTreeWidgetItem* checkbox);

  /// Constructs a MapLayer and sets its type to ELEVATION_LAYER.
  MapLayer(osgEarth::ElevationLayer* elevationLayer,
    MapLayerGroup* parentMapLayerGroup, QTreeWidgetItem* checkbox);

  /// Constructs a MapLayer and sets its type to MODEL_LAYER.
  MapLayer(osgEarth::ModelLayer* modelLayer,
    MapLayerGroup* parentMapLayerGroup, QTreeWidgetItem* checkbox);

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

  /// The group this map layer belongs to.
  MapLayerGroup* mapLayerGroup;

  /// Checkbox in the menu related to this map layer.
  QTreeWidgetItem* listItem;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPLAYER_H_
