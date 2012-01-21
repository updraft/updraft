#include "basicsetting.h"

namespace Updraft {
namespace Core {

BasicSetting::~BasicSetting() {
  // If item is not valid anymore, no need to unregister the setting
  if (item) {
    settingsManager->unregisterSetting(item, this);
  }
}

QVariant BasicSetting::get() {
  if (item) {
    return item->data(Qt::EditRole);
  } else {
    return QVariant();
  }
}

void BasicSetting::set(const QVariant& newValue) {
  if (item) {
    item->setData(newValue);
  }
}

void BasicSetting::callOnValueChanged(QObject* slotOwner, const char* slot) {
  QObject::connect(this, SIGNAL(valueChanged()), slotOwner, slot);
}

BasicSetting::BasicSetting(QStandardItem* settingItem, SettingsManager* mgr) {
  item = settingItem;
  settingsManager = mgr;
}

}  // End namespace Core
}  // End namespace Updraft

