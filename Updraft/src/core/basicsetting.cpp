#include "basicsetting.h"

namespace Updraft {
namespace Core {

BasicSetting::~BasicSetting() {
  // If item is not valid anymore, no need to remove it from parent
  if (itemValid) {
    QStandardItem* parent = item->parent();
    parent->removeRow(item->row());
  }
}

QVariant BasicSetting::get() {
  if (itemValid) {
    return item->parent()->data(Qt::EditRole);
  } else {
    return QVariant();
  }
}

void BasicSetting::set(const QVariant& newValue) {
  if (itemValid) {
    item->parent()->setData(newValue);

    // Notify all other instances of this setting that a value changed
    QStandardItem* parent = item->parent();
    for (int row = 0; row < parent->rowCount(); ++row) {
      QStandardItem* instanceItem = parent->child(row);
      if (instanceItem != item) {
        QVariant instance = instanceItem->data(Qt::EditRole);
        BasicSetting* instancePtr = (BasicSetting*)instance.value<void*>();
        instancePtr->emitValueChanged();
      }
    }
  }
}

BasicSetting::BasicSetting(QStandardItem* settingItem) {
  QStandardItem* ptrItem = new QStandardItem();
  ptrItem->setData(qVariantFromValue((void*)this), Qt::EditRole);

  settingItem->appendRow(ptrItem);
  item = ptrItem;
  itemValid = true;
}

}  // End namespace Core
}  // End namespace Updraft

