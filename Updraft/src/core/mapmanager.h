#ifndef UPDRAFT_SRC_CORE_MAPMANAGER_H_
#define UPDRAFT_SRC_CORE_MAPMANAGER_H_

#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <osgEarthUtil/SkyNode>
#include <osgViewer/Viewer>
#include <QString>
#include <QVector>

#include "ui/maplayergroup.h"
#include "mapmanipulator.h"
#include "mapmapobject.h"
#include "updraft.h"

namespace Updraft {
namespace Core {

/// Map manager is a class providing functions
/// to manipulate with the map.
class MapManager {
  Q_DECLARE_TR_FUNCTIONS(MapManager)

 public:
  MapManager();

  /// Creates a new MapManager with map created from the earth file.
  explicit MapManager(QString earthFile, QString mapName,
    bool hasSkyNode = false, osgViewer::Viewer* viewer_ = NULL);

  void createMap();
  void destroyMap();

  QString getName();

  osgEarth::MapNode* getMapNode();
  osgEarth::Map* getMap();
  MapManipulator* getManipulator();
  void setManipulator(MapManipulator* newManipulator);
  bool hasElevation();
  MapObject* getMapObject();

  void updateCameraProjection();

  /// Gets all the map layers from the map.
  QVector<MapLayerInterface*> getMapLayers();

  void attach(osg::Group* scene);
  void detach(osg::Group* scene);

 private:
  QString earthFileName;
  QString mapName;
  osg::ref_ptr<osgEarth::MapNode> mapNode;
  osgEarth::Map* map;
  QVector<MapLayerInterface*> mapLayers;
  MapLayerGroupInterface* mapLayerGroup;
  osg::ref_ptr<MapManipulator> manipulator;
  MapMapObject mapObject;

  bool hasSkyNode;
  osg::ref_ptr<osgEarth::Util::SkyNode> skyNode;
  osgViewer::Viewer* viewer;
  bool is3d;

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
