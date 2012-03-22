#include "plotwidget.h"

#include <QtCore/qmath.h>
#include <QDebug>
#include <QPoint>
#include <QPainter>

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = Qt::black;
const QPen PlotWidget::AXES_PEN = QPen(Qt::gray);
const QPen PlotWidget::TICKS_PEN = QPen(QBrush(Qt::gray), 0, Qt::DotLine);
const QPen PlotWidget::PLOT_PEN = QPen(Qt::red);

static const qreal LN10 = qLn(10);

PlotWidget::PlotWidget(IgcInfo* info)
  : info(info) {
  setMinimumSize(2 * PIXEL_OFFSET_X + MIN_TICK_SIZE,
    2 * PIXEL_OFFSET_Y + MIN_TICK_SIZE);
}

int PlotWidget::findTickIncrement(qreal range,
  qreal size, qreal minTickSpacing) {
  qreal tmp = minTickSpacing * range / size;

  qreal increment = qExp(qCeil(qLn(tmp) / LN10) * LN10);

  if (increment * size / range > 2 * minTickSpacing) {
    return increment / 2;
  } else {
    return increment;
  }
}

qreal PlotWidget::placeX(qreal val) {
  return val * (width() - 2 * PIXEL_OFFSET_X) /
    info->maxTime() + PIXEL_OFFSET_X;
}

qreal PlotWidget::placeY(qreal val) {
  return (info->robustMax() - val) * (height() - 2 * PIXEL_OFFSET_Y) /
    (info->robustMax() - info->robustMin()) + PIXEL_OFFSET_Y;
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  // int lengthX = width() - 2 * PIXEL_OFFSET_X;
  // int lengthY = height() - 2 * PIXEL_OFFSET_Y;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), BG_COLOR);

  painter.setPen(TICKS_PEN);

  // Y ticks
  qreal tickIncrementY = findTickIncrement(
    info->robustMax() - info->robustMin(),
    height() - 2 * PIXEL_OFFSET_Y, MIN_TICK_SIZE);

  qreal y = tickIncrementY * qCeil(info->robustMin() / tickIncrementY);
  while (y < info->robustMax()) {
    qreal pixelY = placeY(y);

    painter.drawLine(QPointF(PIXEL_OFFSET_X + 1, pixelY),
      QPointF(width() - PIXEL_OFFSET_X, pixelY));

    y += tickIncrementY;
  }

  // X ticks
  qreal tickIncrementX = findTickIncrement(
    info->maxTime(),
    width() - 2 * PIXEL_OFFSET_X, MIN_TICK_SIZE);

  qreal x = tickIncrementX;
  while (x < info->maxTime()) {
    qreal pixelX = placeX(x);

    painter.drawLine(QPointF(pixelX, PIXEL_OFFSET_Y),
      QPointF(pixelX, height() - PIXEL_OFFSET_Y));

    x += tickIncrementX;
  }

  // Y axis
  painter.setPen(AXES_PEN);
  painter.drawLine(PIXEL_OFFSET_X, PIXEL_OFFSET_Y,
    PIXEL_OFFSET_X, height() - PIXEL_OFFSET_Y);

  // X Axis
  qreal pixelBaseline;
  if (info->robustMin() > 0) {
    pixelBaseline = height() - PIXEL_OFFSET_Y;
  } else if (info->robustMax() < 0) {
    pixelBaseline = PIXEL_OFFSET_Y;
  } else {
    pixelBaseline = placeY(0);
  }
  painter.drawLine(QPointF(PIXEL_OFFSET_X, pixelBaseline),
    QPointF(width() - PIXEL_OFFSET_X, pixelBaseline));

  // The plot itself
  painter.setPen(PLOT_PEN);
  QPointF prevPoint(placeX(info->time(0)), placeY(info->value(0)));
  for (int i = 1; i < info->count(); ++i) {
    QPointF currentPoint(placeX(info->time(i)), placeY(info->value(i)));
    painter.drawLine(prevPoint, currentPoint);

    prevPoint = currentPoint;
  }
}

}  // End namespace IgcViewer
}  // End namespace Updraft
