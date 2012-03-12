#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINT_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINT_H_

#include "../../libraries/util/util.h"

namespace Updraft {

enum WaypointStyle {
  UNKNOWN         = 0,
  NORMAL          = 1,
  AIRFIELDGRASS   = 2,
  OUTLANDING      = 3,
  GLIDERSITE      = 4,
  AIRFIELDSOLID   = 5,
  MTPASS          = 6,
  MTTOP           = 7,
  SENDER          = 8,
  VOR             = 9,
  NDB             = 10,
  COOLTOWER       = 11,
  DAM             = 12,
  TUNNEL          = 13,
  BRIDGE          = 14,
  POWERPLANT      = 15,
  CASTLE          = 16,
  INTERSECTION    = 17
};

struct TurnPoint {
  /// Unique identifier of the turn-point
  QString code;

  /// Readable name of the turn-point
  QString name;

  /// Geographic position of turn-point
  Util::Location location;

  /// Cupfile entry type
  WaypointStyle type;

  /// For turnpoits type 2,3,4,5 = Runway
  /// \{Runway heading in degrees
  int rwyHeading;
  /// Runway length in meters
  float rwyLengthM;
  /// Airport frequency
  float airportFreq;
  /// \}
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINT_H_
