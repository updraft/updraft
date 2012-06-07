#ifndef UPDRAFT_SRC_SETTINGINTERFACE_H_
#define UPDRAFT_SRC_SETTINGINTERFACE_H_

#include <QVariant>

class QObject;

namespace Updraft {

/// Interface to settings value.
class SettingInterface {
 public:
  virtual ~SettingInterface() {}

  /// Retreive the value of the setting.
  virtual QVariant get() = 0;

  /// Set the value of setting.
  virtual void set(const QVariant& value) = 0;

  /// Change description of the setting.
  virtual void setDescription(const QString& newDescription) = 0;

  /// Change  if the setting needs restart to take effect.
  virtual void setNeedsRestart(bool needsRestart) = 0;

  /// Connects to a slot valueChanged() -- value of the setting has been
  /// changed.
  /// \see QObject::connect()
  virtual void callOnValueChanged(QObject* slotOwner, const char* slot) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_SETTINGINTERFACE_H_

