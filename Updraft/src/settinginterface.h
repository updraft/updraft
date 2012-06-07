#ifndef UPDRAFT_SRC_SETTINGINTERFACE_H_
#define UPDRAFT_SRC_SETTINGINTERFACE_H_

#include <QVariant>

class QObject;

namespace Updraft {

/// Interface to a single settings value
class SettingInterface {
 public:
  virtual ~SettingInterface() {}

  /// \return Current value of the associated setting.
  virtual QVariant get() = 0;

  /// Sets the value of the associated setting.
  /// \param value The new value of the setting.
  virtual void set(const QVariant& value) = 0;

  /// Sets the description string for the settings dialog.
  /// \param newDescription The new description of the setting in the dialog.
  virtual void setDescription(const QString& newDescription) = 0;

  /// Sets/unsets the needs_restart flag of the setting.
  /// This flag is used to display an asterisk next to settings that need the
  /// application to restart after their value is changed.
  /// \param needsRestart Whether the flag should be set or unset
  virtual void setNeedsRestart(bool needsRestart) = 0;

  /// Connects a slot that should be called whenever the value changes.
  /// \param slotOwner pointer to the QObject owning the target slot.
  /// \param slot Name of the slot to be called. This can be obtained using the
  ///        SLOT(foo()) syntax.
  virtual void callOnValueChanged(QObject* slotOwner, const char* slot) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_SETTINGINTERFACE_H_

