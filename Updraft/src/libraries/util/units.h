#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_UNITS_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_UNITS_H_

#include "util.h"

namespace Updraft {
namespace Util {
/// Class for units conversion.
class Units {
 public:
  /// Converts meters to feet.
  static qreal metersToFeet(qreal meters) { return (1.0/0.3048) * meters; }

  /// Converts feet to meters.
  static qreal feetToMeters(qreal feet) { return 0.3048 * feet; }

  /// Converts nautical miles to meters
  static qreal nauticlaMilesToMeters(qreal nm) { return 1852 * nm; }

  /// Converts Statute miles to meters
  static qreal statuteMilesToMeters(qreal ml) { return 1609.344 * ml; }

  /// \return Radius of FAI sphere in meters
  static qreal FAISphereRadius() { return 6371000.0; }

  /// \return Equatorial radius of WGS84 ellipsoid in meters
  static qreal WGS84EquatRadius() { return 6378137.0; }

  /// \return Flattening of WGS84 ellipsoid
  static qreal WGS84Flattening() { return 1.0/298.257223563; }
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_UNITS_H_
