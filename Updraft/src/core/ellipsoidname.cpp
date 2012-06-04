#include "ellipsoidname.h"

namespace Updraft {
namespace Core {

EllipsoidName::EllipsoidName() {}

EllipsoidName::EllipsoidName(const QString &str)
  : QString(str) {}

QString EllipsoidName::asQString() const {
  return *static_cast<const QString*>(this);
}

}  // End namespace Core
}  // End namespace Updraft

