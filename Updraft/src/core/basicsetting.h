#ifndef UPDRAFT_SRC_CORE_BASICSETTING_H_
#define UPDRAFT_SRC_CORE_BASICSETTING_H_

#include <QtGui>
#include "settingsmanager.h"
#include "settingsmodel.h"
#include "../settinginterface.h"

namespace Updraft {
namespace Core {

class BasicSetting: public QObject, public SettingInterface {
  Q_OBJECT

 public:
  ~BasicSetting();

  QVariant get();
  void set(const QVariant& newValue);
  void setDescription(const QString& newDescription);

  void setNeedsRestart(bool needsRestart);

  void callOnValueChanged(QObject* slotOwner, const char* slot);

  friend class SettingsManager;
  friend class SettingsDelegate;

 signals:
  void valueChanged();

 private:
  void emitValueChanged() {
    emit valueChanged();
  }

  // Only SettingsManager can construct this class
  BasicSetting(SettingsItem* settingItem, SettingsManager* mgr);

  /// Invalidates the item pointer.
  void invalidate() { item = NULL; }

  /// Pointer to the value item
  SettingsItem* item;

  SettingsManager* settingsManager;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_BASICSETTING_H_

