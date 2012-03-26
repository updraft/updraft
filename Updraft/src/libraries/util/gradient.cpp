#include "gradient.h"

#include <QtCore/qmath.h>

namespace Updraft {
namespace Util {

static const qreal ACHROMATIC_THRESHOLD = 0.005;

Gradient::Gradient(const QColor &c1, const QColor &c2, bool reverseHue) {
  qreal h1, s1, v1, a1;
  qreal h2, s2, v2, a2;

  c1.getHsvF(&h1, &s1, &v1, &a1);
  c2.getHsvF(&h2, &s2, &v2, &a2);

  if (s1 < ACHROMATIC_THRESHOLD || v1 < ACHROMATIC_THRESHOLD) {
    // color1 is achromatic
    hue.set(0, h2);
  } else if (s2 < ACHROMATIC_THRESHOLD || v2 < ACHROMATIC_THRESHOLD) {
    // color 2 is achromatic
    hue.set(0, h1);
  } else {
    qreal a = h2 - h1;
    if (a > 0.5) {
      a = a - 1;
    } else if (a < -0.5) {
      a = a + 1;
    }

    if (reverseHue) {
      a = a - 1;
    }

    hue.set(a, h1);
  }

  sat.set(0, s1, 1, s2);
  val.set(0, v1, 1, v2);
  alpha.set(0, a1, 1, a2);
}

QColor Gradient::get(qreal t) const {
  if (t > 1) {
    t = 1;
  } else if (t < 0) {
    t = 0;
  }

  qreal h = hue.get(t);
  h = h - qFloor(h);

  qreal s = sat.get(t);
  if (s < 0) {
    s = 0;
  } else if (s > 1) {
    s = 1;
  }

  qreal v = val.get(t);
  if (v < 0) {
    v = 0;
  } else if (v > 1) {
    v = 1;
  }

  qreal a = alpha.get(t);
  if (a < 0) {
    a = 0;
  } else if (a > 1) {
    a = 1;
  }

  return QColor::fromHsvF(h, s, v, a);
}

}  // End namespace Util
}  // End namespace Updraft

