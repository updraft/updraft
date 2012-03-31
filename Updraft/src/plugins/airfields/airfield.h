#ifndef UPDRAFT_SRC_PLUGINS_AIRFIELDS_AIRFIELD_H_
#define UPDRAFT_SRC_PLUGINS_AIRFIELDS_AIRFIELD_H_

#include <qcore>
#include "../../libraries/util/util.h"

namespace Updraft {

/// Runway surface
enum RwyType {
  UNKNOWN   = 0,
  GRASS     = 1,
  ASPHALT   = 2,
  CONCRETE  = 3
};

/// Runway structure
struct Runaway {
  /// Runway heading in degrees
  int rwyHeading;
  /// Runway length in meters
  qreal rwyLengthM;
  /// Rwy type
  RwyType type;
};

struct Airport {
  /// Readable name of the turn-point
  QString name;

  /// Geographic position of turn-point
  Util::Location location;

  /// Runways
  QVector<Runway> rwys;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRFIELDS_AIRFIELD_H_
