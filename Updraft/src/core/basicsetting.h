#ifndef UPDRAFT_SRC_CORE_BASICSETTING_H_
#define UPDRAFT_SRC_CORE_BASICSETTING_H_

#include <QtGui>
#include "settingsmanager.h"
#include "settingsmodel.h"
#include "../settinginterface.h"

namespace Updraft {
namespace Core {

/// Implementation of the setting interface.
class BasicSetting: public QObject, public SettingInterface {
  Q_OBJECT

 public:
  ~BasicSetting();

  /// \return Current value of the associated setting.
  QVariant get();

  /// Sets the value of the associated setting.
  /// \param newValue The new value of the setting.
  void set(const QVariant& newValue);

  /// Sets the description string for the settings dialog.
  /// \param newDescription The new description of the setting in the dialog.
  void setDescription(const QString& newDescription);

  /// Sets/unsets the needs_restart flag of the setting.
  /// This flag is used to display an asterisk next to settings that need the
  /// application to restart after their value is changed.
  /// \param needsRestart Whether the flag should be set or unset
  void setNeedsRestart(bool needsRestart);

  /// Connects a slot that should be called whenever the value changes.
  /// \param slotOwner pointer to the QObject owning the target slot.
  /// \param slot Name of the slot to be called. This can be obtained using the
  ///        SLOT(foo()) syntax.
  void callOnValueChanged(QObject* slotOwner, const char* slot);

  friend class SettingsManager;
  friend class SettingsDelegate;

 signals:
  /// This is the signal that is emited when the value of the setting changes.
  void valueChanged();

 private:
  /// A private function that emits the value change signal
  void emitValueChanged() {
    emit valueChanged();
  }

  /// Only SettingsManager can construct this class.
  /// That's why this constructor is private.
  /// \param settingItem Pointer to the associated SettingItem instance in the
  ///        SettingsModel
  /// \param mgr Pointer to the application's settings manager.
  BasicSetting(SettingsItem* settingItem, SettingsManager* mgr);

  /// Invalidates the item pointer.
  void invalidate() { item = NULL; }

  /// Pointer to the value item
  SettingsItem* item;

  /// Internal pointer to the settings manager
  SettingsManager* settingsManager;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_BASICSETTING_H_

