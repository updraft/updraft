#ifndef UPDRAFT_SRC_SETTINGINTERFACE_H_
#define UPDRAFT_SRC_SETTINGINTERFACE_H_

#include <QVariant>

class QObject;

namespace Updraft {

class SettingInterface {
 public:
  virtual ~SettingInterface() {}

  virtual QVariant get() = 0;
  virtual void set(const QVariant& value) = 0;
  virtual void setDescription(const QString& newDescription) = 0;

  virtual void setNeedsRestart(bool needsRestart) = 0;

  virtual void callOnValueChanged(QObject* slotOwner, const char* slot) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_SETTINGINTERFACE_H_

