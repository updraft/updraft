#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_

#include <qmath.h>

#include "util.h"

namespace Updraft {
namespace Util {

/// Class that represents position on (or above) earth.
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

  /// Set latitude from radians.
  void latFromRadians(qreal radians);

  /// Set longitude from radians.
  void lonFromRadians(qreal radians);

  /// Set latitude from degrees, minutes and seconds.
  /// \param d Degrees
  /// \param m Minutes
  /// \param s Seconds
  /// \param sign 'N' for north or 'S' for south.
  void latFromDMS(qreal d, qreal m, qreal s, char sign);

  /// Set longitude from degrees, minutes and seconds.
  /// \param d Degrees
  /// \param m Minutes
  /// \param s Seconds
  /// \param sign 'E' for east or 'W' for west.
  void lonFromDMS(qreal d, qreal m, qreal s, char sign);

  /// Retrieves degrees, minutes and seconds from latitude
  /// \param d Degrees
  /// \param m Minutes
  /// \param s Seconds
  /// \param sign 'N' for north or 'S' for south
  void latToDMS(int *d, int *m, qreal *s, char *sign) const;

  /// Retrieves degrees, minutes and seconds from longitude
  /// \param d Degrees
  /// \param m Minutes
  /// \param s Seconds
  /// \param sign 'E' for east or 'W' for west.
  void lonToDMS(int *d, int *m, qreal *s, char *sign) const;

  /// Return latitude as string.
  /// Uses nice unicode characters for degree marks.
  QString latToString() const;

  /// Return longitude as string.
  /// Uses nice unicode characters for degree marks.
  QString lonToString() const;

 private:
  static QString dmsToString(int d, int m, qreal s, char sign);
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_LOCATION_H_
