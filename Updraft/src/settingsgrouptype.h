#ifndef UPDRAFT_SRC_SETTINGSGROUPTYPE_H_
#define UPDRAFT_SRC_SETTINGSGROUPTYPE_H_

namespace Updraft {

/// This type defines the settings group type. Advanced setting groups are
/// displayed in the settings dialog only upon request, whereas the hidden
/// setting groups are never shown, thus preventing the user from editing
/// them.
enum SettingsGroupType {
  GROUP_VISIBLE,
  GROUP_HIDDEN,
  GROUP_ADVANCED
};

}  // End namespace Updraft

#endif

