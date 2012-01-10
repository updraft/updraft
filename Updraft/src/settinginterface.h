#ifndef UPDRAFT_SRC_SETTINGINTERFACE_H_
#define UPDRAFT_SRC_SETTINGINTERFACE_H_

#include <QVariant>

namespace Updraft {

class SettingInterface: public QObject {
 Q_OBJECT

 public:
  virtual ~SettingInterface() {}

  virtual QVariant get() = 0;
  virtual void set(const QVariant& value) = 0;

 signals:
  void valueChanged();
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_SETTINGINTERFACE_H_

