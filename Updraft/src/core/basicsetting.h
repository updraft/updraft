#ifndef UPDRAFT_SRC_CORE_BASICSETTING_H_
#define UPDRAFT_SRC_CORE_BASICSETTING_H_

#include <QtGui>
#include "settingsmanager.h"
#include "../settinginterface.h"

namespace Updraft {
namespace Core {

class BasicSetting: public SettingInterface {
 Q_OBJECT

 public:
  ~BasicSetting();

  QVariant get();
  void set(const QVariant& newValue);

  friend class SettingsManager;
  friend class SettingsDelegate;
 private:
  // Only SettingsManager can construct this class
  BasicSetting(QStandardItem* settingItem);

  //! Invalidates the item pointer.
  //! This is called to prevent a double-free
  void invalidate() { itemValid = false; }

  void emitValueChanged() {
    emit valueChanged();
  }

  QStandardItem* item;

  //! Says whether item points to a valid item model
  bool itemValid;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_BASICSETTING_H_

