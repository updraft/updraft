#include "colorings.h"

#include <QtCore/qmath.h>

namespace Updraft {
namespace IgcViewer {

DefaultColoring::DefaultColoring(const FixInfo *info,
  const Util::Gradient *gradient)
  : info(info), gradient(gradient) {}

QColor DefaultColoring::color(int i) {
  qreal scaled = (info->value(i) - info->globalRobustMin()) /
    (info->globalRobustMax() - info->globalRobustMin());
  return gradient->get(scaled);
}

SymmetricColoring::SymmetricColoring(
  const FixInfo *info, const Util::Gradient *gradient)
  : DefaultColoring(info, gradient) {}

QColor SymmetricColoring::color(int i) {
  qreal max = qMax(info->globalRobustMax(), -info->globalRobustMin());

  qreal scaled = (info->value(i) + max) / (2 * max);
  return gradient->get(scaled);
}

LocalColoring::LocalColoring(
  const FixInfo *info, const Util::Gradient *gradient)
  : DefaultColoring(info, gradient) {}

QColor LocalColoring::color(int i) {
  qreal scaled = (info->value(i) - info->robustMin()) /
    (info->robustMax() - info->robustMin());
  return gradient->get(scaled);
}

ConstantColoring::ConstantColoring(QColor color)
  : c(color) {}

QColor ConstantColoring::color(int i) {
  return c;
}

}  // End namespace IgcViewer
}  // End namespace Updraft

