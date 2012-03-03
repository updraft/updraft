#ifndef UPDRAFT_SRC_CORE_MAPS_UPDRAFTARCGISTILESOURCE_H_
#define UPDRAFT_SRC_CORE_MAPS_UPDRAFTARCGISTILESOURCE_H_

#include <osg/Image>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <osgEarthSymbology/Geometry>
#include <osgEarthSymbology/GeometryRasterizer>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarth/TileSource>
#include <osgEarth/URI>
#include <osgEarth/Profile>

#include <osg/Notify>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <list>

namespace Updraft {
namespace Core {

class Extent {
 public:
  Extent();
  Extent(double _xmin, double _ymin, double _xmax, double _ymax,
    const std::string& _srs);
  Extent(const Extent& rhs);

  double xMin() const;
  double yMin() const;
  double xMax() const;
  double yMax() const;

  const std::string& srs();

  bool isValid() const;

 private:
  double xmin, ymin, xmax, ymax;
  std::string srs_str;
  bool is_valid;
};

class MapServiceLayer {
 public:
  MapServiceLayer(int id, const std::string& name);
  int getId() const;
  const std::string& getName() const;

 private:
  int id;
  std::string name;
};

typedef std::list<MapServiceLayer> MapServiceLayerList;

/**
 * The tiling scheme defined by the map service.
 */
class TileInfo {
 public:
  TileInfo();
  TileInfo(int tile_size, const std::string& format, int min_level,
    int max_level, int _num_tiles_wide, int _num_tiles_high);
  TileInfo(const TileInfo& rhs);
  bool isValid() const;
  int getTileSize() const;
  const std::string& getFormat() const;
  int getMinLevel() const;
  int getMaxLevel() const;
  int getNumTilesWide() const;
  int getNumTilesHigh() const;

 private:
  std::string format;
  int tile_size;
  int min_level, max_level;
  bool is_valid;
  int num_tiles_wide;
  int num_tiles_high;
};

/**
 * ESRI ArcGIS Server Map Service interface.
 */
class MapService {
 public:
  MapService();

  /**
   * Initializes a map service interface and populates its metadata from the
   * provided REST API URL (e.g.: http://server/ArcGIS/rest/services/MyMapService)
   * Call isValid() to verify success.
   */
  bool init(const std::string& url,
    const osgDB::ReaderWriter::Options* options = 0L);

  /**
   * Returns true if the map service initialized succesfully.
   */
  bool isValid() const;

  bool isTiled() const;

  /** 
   * If isValid() returns false, this method will return the error message.
   */
  const std::string& getError() const;

  /**
   * Gets the data profile associated with this map service.
   */
  const osgEarth::Profile* getProfile() const;

  /**
   * Gets the tile information for this service.
   */
  const TileInfo& getTileInfo() const;

 private:
  bool is_valid;
  std::string url;
  osg::ref_ptr<const osgEarth::Profile> profile;
  std::string error_msg;
  MapServiceLayerList layers;
  bool tiled;
  TileInfo tile_info;

  bool setError(const std::string&);
};

class UpdraftArcGisTileSource : public osgEarth::TileSource {
 public:
  explicit UpdraftArcGisTileSource(const osgEarth::TileSourceOptions& options);
  void initialize(const std::string& referenceURI,
    const osgEarth::Profile* overrideProfile);
  int getPixelsPerTile() const;
  osg::Image* createImage(const osgEarth::TileKey& key,
    osgEarth::ProgressCallback* progress);
  osg::HeightField* createHeightField(const osgEarth::TileKey& key,
    osgEarth::ProgressCallback* progress);
  virtual std::string getExtension() const;

 private:
  const osgEarth::Drivers::ArcGISOptions _options;
  osgEarth::optional<osgEarth::ProfileOptions> _profileConf;
  std::string _map;
  std::string _layer;
  std::string _format;
  MapService _map_service;

  osg::Image* noDataImage;

  /// Number of bytes to compare when comparing
  /// against the empty tile image.
  unsigned int nBytes;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPS_UPDRAFTARCGISTILESOURCE_H_
