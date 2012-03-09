#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_GRADIENT_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_GRADIENT_H_

#include <QColor>

#include "util.h"

namespace Updraft {
namespace Util {

/// Color gradient in HSV space.
class UTIL_EXPORT Gradient {
 public:
  /// Initialize the gradient with end colors.
  Gradient(const QColor &c1, const QColor &c2);

  /// Return the color at given position.
  /// \param t position in the gradient, t >= 0, t <= 1
  QColor get(qreal t) const;

 protected:
  static const qreal ACHROMATIC_THRESHOLD = 0.005;

  qreal hA, hB;
  qreal sA, sB;
  qreal vA, vB;
  qreal aA, aB;
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_GRADIENT_H_

