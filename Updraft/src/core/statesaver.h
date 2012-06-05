#ifndef UPDRAFT_SRC_CORE_STATESAVER_H_
#define UPDRAFT_SRC_CORE_STATESAVER_H_

#include "settingsmanager.h"

namespace Updraft {
namespace Core {

/// Saving and loading application state to settings.
/// Needs settings manager, main window, file type manager and scene manager.
class StateSaver {
 public:
  /// Prepare the state saver, create settings.
  StateSaver();

  /// Save all the values to settings.
  void save();

  /// Load values from settings.
  void load();
 private:
  SettingInterface* windowGeometry;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_STATESAVER_H_

