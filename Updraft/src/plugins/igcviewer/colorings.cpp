#include "colorings.h"

#include <QtCore/qmath.h>

namespace Updraft {
namespace IgcViewer {

DefaultColoring::DefaultColoring(const IgcInfo *info,
  const Util::Gradient *gradient)
  : info(info), gradient(gradient) {}

QColor DefaultColoring::color(int i) {
  qreal scaled = (info->value(i) - info->globalMin()) /
    (info->globalMax() - info->globalMin());
  return gradient->get(scaled);
}

SymmetricColoring::SymmetricColoring(
  const IgcInfo *info, const Util::Gradient *gradient)
  : DefaultColoring(info, gradient) {}

QColor SymmetricColoring::color(int i) {
  qreal max = qMax(info->globalMax(), -info->globalMin());

  qreal scaled = (info->value(i) + max) / (2 * max);
  return gradient->get(scaled);
}

CyclingColoring::CyclingColoring(const IgcInfo* info,
  int count, QColor firstColor)
  : info(info) {
  step = 1 / static_cast<qreal>(count);

  firstColor.getHsvF(&h, &s, &v, &a);
}

QColor CyclingColoring::color(int i) {
  qreal newh = h + step * info->value(i);

  newh = newh - qFloor(newh);

  return QColor::fromHsvF(newh, s, v, a);
}

}  // End namespace IgcViewer
}  // End namespace Updraft

