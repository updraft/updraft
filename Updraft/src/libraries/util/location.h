#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_

#include <qmath.h>

#include "util.h"

namespace Updraft {
namespace Util {

/// Class that represents position on (or above) earth.
/// Latitude, longitude and altitude are specified relative to
/// WGS84 reference ellipsoid (the same that osgEarth uses by default).
/// Latitude and longitude are in degrees, altitude in meters.
class UTIL_EXPORT Location {
 public:
  qreal lat;
  qreal lon;
  qreal alt;

  Location();

  /// Latitude in radians.
  qreal lat_radians() const {
    return M_PI * lat / 180.0;
  }

  /// Longitude in radians.
  qreal lon_radians() const {
    return M_PI * lon / 180.0;
  }

  /// Set latitude from degrees, minutes and seconds.
  /// \param sign 'N' for north or 'S' for south.
  void latFromDMS(qreal d, qreal m, qreal s, char sign);

  /// Set latitude from degrees, minutes and seconds.
  /// \param sign 'E' for east or 'W' for west.
  void lonFromDMS(qreal d, qreal m, qreal s, char sign);

  /// Retrieves degrees, minutes and seconds from latitude
  /// \param sign 'N' for north or 'S' for south
  void latToDMS(int *d, int *m, qreal *s, char *sign) const;

  /// Retrieves degrees, minutes and seconds from longitude
  /// \param sign 'E' for east or 'W' for west.
  void lonToDMS(int *d, int *m, qreal *s, char *sign) const;
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_

