#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPFILE_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPFILE_H_

#include "turnpoint.h"

namespace Updraft {

typedef QList<TurnPoint> TTPList;

/// Interface for a turn-points file.
class TPFile {
 public:
  virtual ~TPFile() {}

  /// \return File name
  virtual QString getFileName() const = 0;

  /// \return Absolute (full) file path
  virtual QString getFilePath() const = 0;

  /// \return List of all turn points contained in file.
  virtual const TTPList& getTurnPoints() const = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPFILE_H_
