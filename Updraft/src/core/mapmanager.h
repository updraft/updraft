#ifndef UPDRAFT_SRC_CORE_MAPMANAGER_H_
#define UPDRAFT_SRC_CORE_MAPMANAGER_H_

#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <QString>
#include <QVector>

#include "maplayer.h"

namespace Updraft {
namespace Core {

/// Map manager is a class providing functions
/// to manipulate with the map.
class MapManager {
 public:
  MapManager();

  /// Creates a new MapManager with map created from the earth file.
  explicit MapManager(QString earthFile);
  ~MapManager();

  osgEarth::MapNode* getMapNode();

  /// Gets all the map layers from the map.
  QVector<MapLayerInterface*> getMapLayers();

 private:
  osgEarth::MapNode* mapNode;
  osgEarth::Map* map;

  /// Gets a list of ImageLayers in the map.
  QVector<osgEarth::ImageLayer*> getImageLayers();

  /// Gets a list of ElevationLayers in the map.
  QVector<osgEarth::ElevationLayer*> getElevationLayers();

  /// Gets a list of ModelLayers in the map.
  QVector<osgEarth::ModelLayer*> getModelLayers();
};

}  // namespace Core
}  // namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPMANAGER_H_
