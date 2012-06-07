#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_LINEARFUNC_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_LINEARFUNC_H_

#include <QColor>

#include "util.h"

namespace Updraft {
namespace Util {

/// Class representing a linear function.
/// Just a convenience to avoid keeping two qreals all the time.
class UTIL_EXPORT LinearFunc {
 public:
  LinearFunc()
    : a(0), b(0) {}

  LinearFunc(qreal slope, qreal yIntercept)
    : a(slope), b(yIntercept) {}

  /// Set the linear function to have value y0 in x0
  /// and y1 in x1.
  LinearFunc(qreal x0, qreal y0, qreal x1, qreal y1) {
    set(x0, y0, x1, y1);
  }

  void set(qreal slope, qreal yIntercept) {
    this->a = slope;
    this->b = yIntercept;
  }

  /// Set the linear function to have value y0 in x0
  /// and y1 in x1.
  void set(qreal x0, qreal y0, qreal x1, qreal y1) {
    a = (y1 - y0) / (x1 - x0);
    b = y0 - a * x0;
  }

  /// Return a composed linear function corresponding
  /// to this->get(other->get(x))
  LinearFunc compose(const LinearFunc& other) const {
    return LinearFunc(a * other.a, a * other.b + b);
  }

  /// Return the value of function in x
  qreal get(qreal x) const {
    return a * x + b;
  }

  /// Return the value of x when the function has the value y
  qreal inverse(qreal y) const {
    return (y - b) / a;
  }

 protected:
  qreal a, b;
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_LINEARFUNC_H_

