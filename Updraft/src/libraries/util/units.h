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
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_UNITS_H_
