#ifndef UPDRAFT_SRC_CORE_MAPMANAGER_H_
#define UPDRAFT_SRC_CORE_MAPMANAGER_H_

#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <QString>
#include <QVector>

#include "ui/maplayergroup.h"
#include "mapmanipulator.h"
#include "mapmapobject.h"
#include "updraft.h"

namespace Updraft {
namespace Core {

/// Map manager is a class providing functions
/// to manipulate with a single map.
class MapManager {
  Q_DECLARE_TR_FUNCTIONS(MapManager)

 public:
  MapManager();

  /// Creates a new MapManager with map created from the earth file.
  /// \param earthFile The path to the earth file where the map for this map.
  ///                  manager is specified.
  /// \param mapName Name of the map to be displayed in the left pane.
  explicit MapManager(QString earthFile, QString mapName);

  /// Creates the map node
  void createMap();

  /// Destroys the map node
  void destroyMap();

  /// Returns the map name.
  /// \return Name of the map as displayed in the left pane.
  QString getName();

  /// Gets the osgEarth map node for this map.
  /// \return Map node that represents this map.
  osgEarth::MapNode* getMapNode();

  /// Gets the osgEarth map.
  /// \return osgEarth map representing this map.
  osgEarth::Map* getMap();

  /// Binds a map manipulator to this map.
  /// \param newManipulator The new manipulator to be set for this map.
  void bindManipulator(MapManipulator* newManipulator);

  /// Tells whether the map has any elevation data.
  /// \return Whether the map has elevation data.
  bool hasElevation();

  /// Returns the MapObject representing this map.
  /// The MapObject is used for clicking into the map.
  /// \return MapObject that represents this map.
  MapObject* getMapObject();

  /// Gets all the map layers from the map.
  /// \return QVector of all child map layers.
  QVector<MapLayerInterface*> getMapLayers();

  /// Attaches this map's map node to the scene.
  /// \param scene The scene to which the map node should be attached
  void attach(osg::Group* scene);

  /// Detaches this map's map node from the scene.
  /// \param scene The scene from which the map node should be detached
  void detach(osg::Group* scene);

 private:
  /// Name of the earth file from which the map was loaded.
  QString earthFileName;

  /// Name of the map displayed in the left pane.
  QString mapName;

  /// Referenced pointer to this map's map node.
  osg::ref_ptr<osgEarth::MapNode> mapNode;

  /// Pointer to the osgEarth::Map object representing this map.
  osgEarth::Map* map;

  /// The map layers of this map.
  QVector<MapLayerInterface*> mapLayers;

  /// Group of all the map layers of this map.
  MapLayerGroupInterface* mapLayerGroup;

  /// Map object used for clicking on this map.
  MapMapObject mapObject;

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
