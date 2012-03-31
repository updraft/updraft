#include "plotaxes.h"

#include <QtCore/qmath.h>
#include <QDebug>
#include <QLayout>

namespace Updraft {
namespace IgcViewer {

const QPen PlotAxes::AXES_PEN = QPen(QBrush(Qt::gray), 2);
const QPen PlotAxes::TICKS_PEN = QPen(QBrush(Qt::gray), 0, Qt::DotLine);
const QPen AxisLabel::LABEL_PEN = QPen(Qt::yellow);

static const qreal LN10 = qLn(10);

Qt::Orientations PlotAxes::expandingDirections() const {
  return Qt::Vertical | Qt::Horizontal;
}

QRect PlotAxes::geometry() const {
  return rect;
}

bool PlotAxes::isEmpty() const {
  return false;
}

QSize PlotAxes::maximumSize() const {
  return QSize(65536, 65536);
}

QSize PlotAxes::minimumSize() const {
  return QSize(TICK_SPACING_X, TICK_SPACING_Y);
}

QSize PlotAxes::sizeHint() const {
  return QSize(65536, 100);
}

void PlotAxes::setGeometry(const QRect& rect) {
  this->rect = rect;

  linX.set(0, rect.left(), maxTime, rect.right());
  linY.set(min, rect.bottom(), max, rect.top());

  qreal tick;

  tick = findTickIncrement(maxTime, rect.width(), TICK_SPACING_X);
  tickCountX = qFloor(maxTime / tick) + 1;
  tickX = linX.compose(Util::LinearFunc(tick, 0));

  tick = findTickIncrement(max - min, rect.height(), TICK_SPACING_Y);
  first = tick * qCeil(min / tick);
  tickCountY = qFloor((max - first) / tick) + 1;
  tickY = linY.compose(Util::LinearFunc(tick, first));
  step = tick;

  if (min > 0) {
    base = rect.bottom();
  } else if (max < 0) {
    base = rect.top();
  } else {
    base = linY.get(0);
  }
}

qreal PlotAxes::placeX(qreal x) {
  return linX.get(x);
}

qreal PlotAxes::placeY(qreal y) {
  return linY.get(y);
}

void PlotAxes::setLimits(qreal min, qreal max, qreal maxTime) {
  this->min = min;
  this->max = max;
  this->maxTime = maxTime;

  setGeometry(rect);
}

int PlotAxes::findTickIncrement(qreal range, qreal size, qreal minTickSpacing) {
  qreal tmp = minTickSpacing * range / size;

  qreal logIncrement = qCeil(qLn(tmp) / LN10);
  qreal increment = qExp(logIncrement * LN10);

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

  painter->drawLine(QPointF(rect.topLeft()), QPointF(rect.bottomLeft()));
  painter->drawLine(QPointF(rect.left(), base), QPointF(rect.right(), base));
}

qreal PlotAxes::getStep() {
  return step;
}

qreal PlotAxes::getFirst() {
  return first;
}

int PlotAxes::getVerticalTicks() {
  return tickCountY;
}

// Axis Labels:

Qt::Orientations AxisLabel::expandingDirections() const {
  return Qt::Vertical;
}

QRect AxisLabel::geometry() const {
  return rect;
}

bool AxisLabel::isEmpty() const {
  return false;
}

QSize AxisLabel::maximumSize() const {
  return QSize(65536, 65536);
}

QSize AxisLabel::minimumSize() const {
  return QSize(MIN_WIDTH, MIN_HEIGHT);
}

QSize AxisLabel::sizeHint() const {
  return QSize(100, 100);
}

void AxisLabel::setGeometry(const QRect& rect_) {
  this->rect = rect_;
}

AxisLabel::AxisLabel(PlotAxes* axis_) {
  axis = axis_;
}

void AxisLabel::draw(QPainter *painter) {
  painter->fillRect(rect, bg);
  painter->setPen(LABEL_PEN);
  qreal step = axis->getStep();
  qreal first = axis->getFirst();
  int nLabels = axis->getVerticalTicks();
  for (int i = 0; i < nLabels; ++i) {
    qreal pixelY = axis->placeY(first+step*i);

    QString text;
    text.setNum(first+step*i, 3, 0);
    QRect labelRect(0, pixelY-TEXT_HEIGHT/2,
      rect.right(), pixelY+TEXT_HEIGHT/2);
    painter->drawText(labelRect, Qt::AlignRight, text);
  }
}

}  // End namespace IgcViewer
}  // End namespace Updraft
