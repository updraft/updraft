#include "plotaxes.h"

#include <QtCore/qmath.h>
#include <QDebug>

namespace Updraft {
namespace IgcViewer {

const QPen PlotAxes::AXES_PEN = QPen(Qt::gray);
const QPen PlotAxes::TICKS_PEN = QPen(QBrush(Qt::gray), 0, Qt::DotLine);

static const qreal LN10 = qLn(10);

QPointF PlotAxes::placePoint(qreal x, qreal y) {
  return QPointF(linX.get(x), linY.get(y));
}

void PlotAxes::setLimits(
  const QRectF& rect, qreal min, qreal max, qreal maxTime) {

  this->rect = rect;
  
  linX.set(0, rect.left(), maxTime, rect.right());
  linY.set(min, rect.bottom(), max, rect.top());

  qreal tick;

  tick = findTickIncrement(maxTime, rect.width(), TICK_SPACING_X);
  tickCountX = qFloor(maxTime / tick) + 1;
  tickX = linX.compose(Util::LinearFunc(tick, 0));

  tick = findTickIncrement(max - min, rect.height(), TICK_SPACING_Y);
  qreal first = tick * qCeil(min / tick);
  tickCountY = qFloor((max - first) / tick) + 1;
  tickY = linY.compose(Util::LinearFunc(tick, first));

  if (min > 0) {
    base = rect.bottom();
  } else if (max < 0) {
    base = rect.top();
  } else {
    base = linY.get(0);
  }
}

int PlotAxes::findTickIncrement(qreal range, qreal size, qreal minTickSpacing) {
  qreal tmp = minTickSpacing * range / size;

  qreal increment = qExp(qCeil(qLn(tmp) / LN10) * LN10);

  if (increment * size / range > 2 * minTickSpacing) {
    return increment / 2;
  } else {
    return increment;
  }
}

void PlotAxes::draw(QPainter *painter) {
  // Ticks
  painter->setPen(TICKS_PEN);

  for (int i = 0; i < tickCountX; ++i) {
    qreal pixelX = tickX.get(i);

    painter->drawLine(QPointF(pixelX, rect.top()),
      QPointF(pixelX, rect.bottom()));
  }

  for (int i = 0; i < tickCountY; ++i) {
    qreal pixelY = tickY.get(i);

    painter->drawLine(QPointF(rect.left(), pixelY),
      QPointF(rect.right(), pixelY));
  }

  // Axes
  painter->setPen(AXES_PEN);

  painter->drawLine(rect.topLeft(), rect.bottomLeft());
  painter->drawLine(QPointF(rect.left(), base), QPointF(rect.right(),base));
}

}  // End namespace IgcViewer
}  // End namespace Updraft
