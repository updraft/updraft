#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_UNITS_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_UNITS_H_

#include <math.h>

#include "util.h"

namespace Updraft {
namespace Util {
/// PI/180 constant
static const double DEG_TO_RAD = 0.017453292519943295769236907684886;
static const double RAD_TO_DEG = 1/DEG_TO_RAD;

/// Nautical miles to meter const
static const int    NM_TO_M = 1852;
static const double M_TO_NM = 1./1852;

/// Statute miles to meters
static const double ML_TO_M = 1609.344;

/// Foot to meters const
static const double FT_TO_M = 0.3048;
static const double M_TO_FT = 1/0.3048;

/// Earth's quatratic mean radius for WGS-84
static const double EARTH_RADIUS_IN_METERS = 6372797.560856;

/// 2pi
static const double M_2PI = 2*M_PI;

/// Class for units conversion.
class Units {
 public:
  /// Converts meters to feet.
  static qreal metersToFeet(qreal meters) { return (1.0/0.3048) * meters; }

  /// Converts feet to meters.
  static qreal feetToMeters(qreal feet) { return 0.3048 * feet; }
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_UNITS_H_
