#ifndef UPDRAFT_SRC_CORE_BASICSETTING_H_
#define UPDRAFT_SRC_CORE_BASICSETTING_H_

#include <QtGui>
#include "settingsmanager.h"
#include "../settinginterface.h"

namespace Updraft {
namespace Core {

class BasicSetting: public SettingInterface {
 public:
  ~BasicSetting() {};

  QVariant get();
  void set(const QVariant& newValue);

  friend class SettingsManager;
 private:
  // Only SettingsManager can construct this class
  BasicSetting(QStandardItem* theItem): item(theItem) {}

  QStandardItem* item;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_BASICSETTING_H_

