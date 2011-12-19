#ifndef UPDRAFT_SRC_CORE_MAPMANAGER_H_
#define UPDRAFT_SRC_CORE_MAPMANAGER_H_

#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <QString>
#include <QVector>

namespace Updraft {
namespace Core {

class MapManager {
 public:
  MapManager();
  explicit MapManager(QString earthFile);
  ~MapManager();

  osgEarth::MapNode* getMapNode();
  osgEarth::MapNode* createMap(QString earthFile);

 private:
  osgEarth::MapNode* mapNode;
  osgEarth::Map* map;
  QVector<osgEarth::ImageLayer*> imageLayers;
  QVector<osgEarth::ElevationLayer*> elevationLayers;
};

}  // namespace Core
}  // namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPMANAGER_H_
