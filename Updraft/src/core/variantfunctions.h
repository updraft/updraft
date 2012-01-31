#ifndef UPDRAFT_SRC_CORE_VARIANTFUNCTIONS_H_
#define UPDRAFT_SRC_CORE_VARIANTFUNCTIONS_H_

#include <QtGui>

namespace Updraft {
namespace Core {

inline bool variantsEqual(const QVariant& var1, const QVariant& var2) {
  if (var1.type() == QVariant::Double && var2.type() == QVariant::Double) {
    return qFuzzyCompare(var1.toDouble(), var2.toDouble());
  }

  return var1 == var2;
}

/// This function is defined so that we can control conversion to
/// a string for any type that a variant can hold.
inline QString variantToString(const QVariant& var) {
  // switch (var.type()) {
    // default:
      return var.toString();
    // break;
  // }
}

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_VARIANTFUNCTIONS_H_

