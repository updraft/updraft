#include "plotwidget.h"

#include <QDebug>
#include <QPainter>

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = Qt::black;
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

void PlotWidget::resizeEvent(QResizeEvent* resizeEvent) {
  axes.setLimits(
    rect().adjusted(OFFSET_X, OFFSET_Y, -OFFSET_X, -OFFSET_Y),
    info->robustMin(), info->robustMax(), info->maxTime());
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  // int lengthX = width() - 2 * PIXEL_OFFSET_X;
  // int lengthY = height() - 2 * PIXEL_OFFSET_Y;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), BG_COLOR);

  axes.draw(&painter);

  painter.setPen(PLOT_PEN);

  QPointF prevPoint = axes.placePoint(info->time(0), info->value(0));
  for (int i = 1; i < info->count(); ++i) {
    QPointF currentPoint = axes.placePoint(info->time(i), info->value(i));
    painter.drawLine(prevPoint, currentPoint);

    prevPoint = currentPoint;
  }
}

}  // End namespace IgcViewer
}  // End namespace Updraft
