#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_

#include <QtGlobal>

namespace Updraft {
namespace Util {

/// Class that represents position on (or above) earth.
/// Latitude, longitude and altitude are specified relative to
/// WGS84 reference ellipsoid (the same that osgEarth uses by default).
class Location {
 public:
  qreal lat;
  qreal lon;
  qreal alt;

  /// Set latitude from degrees, minutes and seconds.
  /// \param sign 'N' for north or 'S' for south.
  void latFromDMS(qreal d, qreal m, qreal s, char sign);

  /// Set latitude from degrees, minutes and seconds.
  /// \param sign 'E' for east or 'W' for west.
  void lonFromDMS(qreal d, qreal m, qreal s, char sign);
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_

