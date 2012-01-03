#include "basicsetting.h"

namespace Updraft {
namespace Core {

QVariant BasicSetting::get() {
  return item->data();
}

void BasicSetting::set(const QVariant& newValue) {
  item->setData(newValue);
}

}  // End namespace Core
}  // End namespace Updraft

