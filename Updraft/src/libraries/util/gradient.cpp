#include "gradient.h"

#include <QtCore/qmath.h>
#include <QDebug>

namespace Updraft {
namespace Util {

Gradient::Gradient(const QColor &c1, const QColor &c2) {
  qreal h1, s1, v1, a1;
  qreal h2, s2, v2, a2;

  c1.getHsvF(&h1, &s1, &v1, &a1);
  c2.getHsvF(&h2, &s2, &v2, &a2);

  if (s1 < ACHROMATIC_THRESHOLD || v1 < ACHROMATIC_THRESHOLD) {
    // color1 is achromatic
    hA = 0;
    hB = h2;
  } else if (s2 < ACHROMATIC_THRESHOLD || v2 < ACHROMATIC_THRESHOLD) {
    // color 2 is achromatic
    hA = 0;
    hB = h1;
  } else {
    hA = h2 - h1;
    if (hA > 0.5) {
      hA = hA - 1;
    } else if (hA < -0.5) {
      hA = hA + 1;
    }
    hB = h1;
  }

  sB = c1.saturationF();
  sA = c2.saturationF() - sB;

  vB = c1.valueF();
  vA = c2.valueF() - vB;

  aB = c1.alphaF();
  aA = c2.alphaF() - aB;
}

QColor Gradient::get(qreal t) const {
  qreal h = hA * t + hB;
  h = h - qFloor(h);

  qreal s = sA * t + sB;
  if (s < 0) {
    s = 0;
  } else if (s > 1) {
    s = 1;
  }

  qreal v = sA * t + vB;
  if (v < 0) {
    v = 0;
  } else if (v > 1) {
    v = 1;
  }

  qreal a = aA * t + aB;
  if (a < 0) {
    a = 0;
  } else if (a > 1) {
    a = 1;
  }

  return QColor::fromHsvF(h, s, v, a);
}

}  // End namespace Util
}  // End namespace Updraft

