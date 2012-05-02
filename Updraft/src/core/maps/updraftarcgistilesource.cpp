#include "updraftarcgistilesource.h"

#include <limits.h>
#include <QDebug>
#include <QApplication>
#include <QString>

#include <osgEarth/HTTPClient>
#include <osgEarth/JsonUtils>
#include <osgEarth/Registry>
#include <osg/Notify>

#include <sstream>

namespace Updraft {
namespace Core {

Extent::Extent() : is_valid(false) {
}

Extent::Extent(double _xmin, double _ymin, double _xmax, double _ymax,
  const std::string &_srs) :
  xmin(_xmin), ymin(_ymin), xmax(_xmax), ymax(_ymax), srs_str(_srs),
  is_valid(true) {
}

Extent::Extent(const Extent &rhs)
  : xmin(rhs.xmin), ymin(rhs.ymin), xmax(rhs.xmax), ymax(rhs.ymax),
  srs_str(rhs.srs_str), is_valid(rhs.is_valid) {
}

double Extent::xMax() const {
  return xmax;
}

double Extent::yMax() const {
  return ymax;
}

double Extent::xMin() const {
  return xmin;
}

double Extent::yMin() const {
  return ymin;
}

const std::string& Extent::srs() {
  return srs_str;
}

bool Extent::isValid() const {
  return is_valid;
}

MapServiceLayer::MapServiceLayer(int in_id, const std::string& in_name)
  : id(in_id), name(in_name) {
}

int MapServiceLayer::getId() const {
  return id;
}

const std::string& MapServiceLayer::getName() const {
  return name;
}

TileInfo::TileInfo()
  : is_valid(false) {
}

TileInfo::TileInfo(int _tile_size, const std::string& _format, int _min_level,
  int _max_level, int _num_tiles_wide, int _num_tiles_high)
  : format(_format),
  tile_size(_tile_size),
  min_level(_min_level),
  max_level(_max_level),
  is_valid(true),
  num_tiles_wide(_num_tiles_wide),
  num_tiles_high(_num_tiles_high) {
}

TileInfo::TileInfo(const TileInfo& rhs)
  : format(rhs.format),
  tile_size(rhs.tile_size),
  min_level(rhs.min_level),
  max_level(rhs.max_level),
  is_valid(rhs.is_valid),
  num_tiles_wide(rhs.num_tiles_wide),
  num_tiles_high(rhs.num_tiles_high) {
}

bool TileInfo::isValid() const {
  return is_valid;
}

int TileInfo::getTileSize() const {
  return tile_size;
}

const std::string& TileInfo::getFormat() const {
  return format;
}

int TileInfo::getMinLevel() const {
  return min_level;
}

int TileInfo::getMaxLevel() const {
  return max_level;
}

int TileInfo::getNumTilesWide() const {
  return num_tiles_wide;
}

int TileInfo::getNumTilesHigh() const {
  return num_tiles_high;
}

MapService::MapService()
  : is_valid(false),
  tiled(false) {
}

bool MapService::isValid() const {
  return is_valid;
}

bool MapService::isTiled() const {
  return tiled;
}

const osgEarth::Profile* MapService::getProfile() const {
  return profile.get();
}

const TileInfo& MapService::getTileInfo() const {
  return tile_info;
}

bool MapService::init(const std::string& _url,
  const osgDB::ReaderWriter::Options* options) {
  url = _url;
  std::string sep = url.find("?") == std::string::npos ? "?" : "&";
  // request the data in JSON format
  std::string json_url = url + sep + std::string("f=pjson");
  osgEarth::Drivers::HTTPResponse response =
    osgEarth::Drivers::HTTPClient::get(json_url, options);
  if (!response.isOK())
    return setError("Unable to read metadata from ArcGIS service");

  osgEarth::Json::Value doc;
  osgEarth::Json::Reader reader;
  if (!reader.parse(response.getPartStream(0), doc))
    return setError("Unable to parse metadata; invalid JSON");

  // Read the profile. We are using "fullExtent";
  // perhaps an option to use "initialExtent" instead?
  double xmin = doc["fullExtent"].get("xmin", 0).asDouble();
  double ymin = doc["fullExtent"].get("ymin", 0).asDouble();
  double xmax = doc["fullExtent"].get("xmax", 0).asDouble();
  double ymax = doc["fullExtent"].get("ymax", 0).asDouble();
  int srs = doc["fullExtent"].get("spatialReference",
    osgEarth::Json::Value::null).get("wkid", 0).asInt();

  // Assumes the SRS is going to be an EPSG code
  std::stringstream ss;
  ss << "epsg:" << srs;

  if (!(xmax > xmin && ymax > ymin && srs != 0)) {
    return setError("Map service does not define a full extent");
  }

  // Read the layers list
  osgEarth::Json::Value j_layers = doc["layers"];
  if (j_layers.empty())
    return setError("Map service contains no layers");

  for (unsigned int i = 0; i < j_layers.size(); i++) {
    osgEarth::Json::Value layer = j_layers[i];
    int id = i;  // layer.get("id", -1).asInt();
    std::string name = layer["name"].asString();

    if (id >= 0 && !name.empty()) {
      layers.push_back(MapServiceLayer(id, name));
    }
  }

  tiled = false;
  std::string format = "png";
  int tile_rows = 256;
  int tile_cols = 256;
  int min_level = 25;
  int max_level = 0;
  int num_tiles_wide = 1;
  int num_tiles_high = 1;

  // Read the tiling schema
  osgEarth::Json::Value j_tileinfo = doc["tileInfo"];
  if (!j_tileinfo.empty()) {
    tiled = true;

    // TODO(OSGE) : what do we do if the width <> height?
    tile_rows = j_tileinfo.get("rows", 0).asInt();
    tile_cols = j_tileinfo.get("cols", 0).asInt();
    if (tile_rows <= 0 && tile_cols <= 0)
      return setError("Map service tile size not specified");

    format = j_tileinfo.get("format", "").asString();
    if (format.empty())
      return setError
      ("Map service tile schema does not specify an image format");

    osgEarth::Json::Value j_levels = j_tileinfo["lods"];
    if (j_levels.empty())
      return setError("Map service tile schema contains no LODs");

    min_level = INT_MAX;
    max_level = 0;
    for (unsigned int i = 0; i < j_levels.size(); i++) {
      int level = j_levels[i].get("level", -1).asInt();
      if (level >= 0 && level < min_level)
        min_level = level;
      if (level >= 0 && level > max_level)
        max_level = level;
    }

    if (j_levels.size() > 0) {
      int l = j_levels[0u].get("level", -1).asInt();
      double res = j_levels[0u].get("resolution", 0.0).asDouble();
      num_tiles_wide = osg::round((xmax - xmin) / (res * tile_cols));
      num_tiles_high = osg::round((ymax - ymin) / (res * tile_cols));

      // In case the first level specified isn't level 0,
      // compute the number of tiles at level 0
      for (int i = 0; i < l; i++) {
        num_tiles_wide /= 2;
        num_tiles_high /= 2;
      }

      // profile.setNumTilesWideAtLod0(num_tiles_wide);
      // profile.setNumTilesHighAtLod0(num_tiles_high);
    }
  }

  std::string ssStr;
  ssStr = ss.str();

  osg::ref_ptr<osgEarth::SpatialReference> spatialReference =
    osgEarth::SpatialReference::create(ssStr);
  if (spatialReference->isGeographic()) {
    // If we have a geographic SRS, just use the geodetic profile
    profile = osgEarth::Registry::instance()->getGlobalGeodeticProfile();
  } else {
    if (spatialReference->isMercator()) {
      // If we have a mercator SRS, just use the mercator profile
      profile = osgEarth::Registry::instance()->getGlobalMercatorProfile();
    } else {
      // It's not geodetic or mercator, so try to use the full extent
      profile = osgEarth::Profile::create(
        spatialReference.get(),
        xmin, ymin, xmax, ymax,
        num_tiles_wide,
        num_tiles_high);
    }
  }

  // now we're good.
  tile_info = TileInfo(tile_rows, format, min_level, max_level,
    num_tiles_wide, num_tiles_high);
  is_valid = true;
  return is_valid;
}

bool MapService::setError(const std::string& msg) {
  error_msg = msg;
  return false;
}

const std::string& MapService::getError() const {
  return error_msg;
}

UpdraftArcGisTileSource::UpdraftArcGisTileSource
  (const osgEarth::TileSourceOptions &options) :
  osgEarth::TileSource(options),
  _options(options),
  _profileConf(osgEarth::ProfileOptions()) {
  // TODO(OSGE): allow single layers vs. "fused view"
  if (_layer.empty())
    _layer = "_alllayers";  // default to the AGS "fused view"

  // TODO(OSGE): detect the format
  if (_format.empty())
    _format = "png";

  osgEarth::URI url = _options.url().value();
  // Add the token if necessary
  if (_options.token().isSet()) {
    std::string token = _options.token().value();
    if (!token.empty()) {
      std::string sep = url.full().find("?") == std::string::npos ? "?" : "&";
      url = url.append(sep + std::string("token=") + token);
    }
  }

  // read metadata from the server
  if (!_map_service.init(url.full())) {
    OE_WARN << "[osgearth] [ArcGIS] map service initialization failed: "
      << _map_service.getError() << std::endl;
  }

  // read the no data image and set number of bytes to compare
  nBytes = 50u;
  QString pathToEmptyTile =
    QCoreApplication::applicationDirPath() + "/data/empty_tile.png";
  noDataImage = osgDB::readImageFile(pathToEmptyTile.toStdString());
  if ((noDataImage != NULL) && (noDataImage->getImageSizeInBytes() < nBytes)) {
    qDebug() << "The empty tile image is smaller than the number " <<
      "of bytes we want to compare.\n"
      << "Comparing the whole image.";
    nBytes = noDataImage->getImageSizeInBytes();
  }
}

void UpdraftArcGisTileSource::initialize(const osgDB::Options* dbOptions,
  const osgEarth::Profile* overrideProfile) {
  _dbOptions = dbOptions;
  const osgEarth::Profile* profile = NULL;
  if (_profileConf.isSet()) {
    profile = osgEarth::Profile::create(_profileConf.get());
  } else {
    if (overrideProfile) {
      profile = overrideProfile;
    } else {
      if (_map_service.getProfile()) {
        profile = _map_service.getProfile();
      } else {
        profile = osgEarth::Registry::instance()->getGlobalGeodeticProfile();
      }
    }
  }

  // Set the profile
  setProfile(profile);
}

int UpdraftArcGisTileSource::getPixelsPerTile() const {
  return _map_service.getTileInfo().getTileSize();
}

osg::Image* UpdraftArcGisTileSource::createImage(const osgEarth::TileKey& key,
  osgEarth::ProgressCallback* progress) {
  std::stringstream buf;

  int level = key.getLevelOfDetail();

  unsigned int tile_x, tile_y;
  key.getTileXY(tile_x, tile_y);

  std::string f = _map_service.getTileInfo().getFormat();
  std::transform(f.begin(), f.end(), f.begin(), tolower);
  if (f.length() > 3 && f.substr(0, 3) == "png")
    f = "png";

  if (_map_service.isTiled()) {
    buf << _options.url()->full() << "/tile"
      << "/" << level
      << "/" << tile_y
      << "/" << tile_x << "." << f;
  } else {
    const osgEarth::GeoExtent& ex = key.getExtent();

    buf << std::setprecision(16)
      << _options.url()->full() << "/export"
      << "?bbox=" << ex.xMin() << "," << ex.yMin() << ","
      << ex.xMax() << "," << ex.yMax()
      << "&format=" << f
      << "&size=256,256"
      << "&transparent=true"
      << "&f=image"
      << "&" << "." << f;
  }

  // Add the token if necessary
  if (_options.token().isSet()) {
    std::string token = _options.token().value();
    if (!token.empty()) {
      std::string str;
      str = buf.str();
      std::string sep = str.find("?") == std::string::npos ? "?" : "&";
      buf << sep << "token=" << token;
    }
  }

  // osg::ref_ptr<osg::Image> image;
  osg::Image* image;
  std::string bufStr;
  bufStr = buf.str();
  // osgEarth::Drivers::HTTPClient::readImageFile(bufStr, image, 0L, progress);
  image = osgEarth::URI(bufStr).readImage(0L, osgEarth::CachePolicy::NO_CACHE,
    progress).releaseImage();

  // check whether the noDataImage is loaded and
  // if the image has minimum size to do the comparison
  if (noDataImage == NULL) {
    return image;
  }
  if (image == NULL) {
    return NULL;
  }
  if ((image->getImageSizeInBytes() < nBytes)) {
    return image;
  }

  // check whether the image is not grey image
  bool valid = false;
  unsigned char* imageData = image->data();
  unsigned char* emptyData = noDataImage->data();
  for (unsigned int i = 0; i < nBytes; i++) {
    if (*imageData != *emptyData) {
      valid = true;
      break;
    }
    imageData++;
    emptyData++;
  }
  if (valid) {
    return image;
  } else {
    // qDebug() << "empty tile";
    return NULL;
  }
}

osg::HeightField* UpdraftArcGisTileSource::createHeightField
  (const osgEarth::TileKey& key, osgEarth::ProgressCallback* progress) {
  return NULL;
}

std::string UpdraftArcGisTileSource::getExtension() const {
  return _format;
}

}  // End namespace Core
}  // End namespace Updraft
