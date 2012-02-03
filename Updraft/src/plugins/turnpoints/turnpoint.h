#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINT_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINT_H_

#include "../../libraries/util/util.h"

namespace Updraft {

struct TurnPoint {
  /// Unique identifier of the turn-point
  QString code;

  /// Readable name of the turn-point
  QString name;

  /// Geographic position of turn-point
  Util::Location location;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINT_H_
