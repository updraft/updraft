#include "plotwidget.h"

#include <QDebug>
#include <QPainter>

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = Qt::black;
const QPen PlotWidget::PLOT_PEN = QPen(Qt::red);

static const qreal LN10 = qLn(10);

PlotWidget::PlotWidget(IgcInfo* info)
  : info(info) {}

void PlotWidget::resizeEvent(QResizeEvent* resizeEvent) {
  axes.setLimits(
    rect().adjusted(OFFSET_X, OFFSET_Y, -OFFSET_X, -OFFSET_Y),
    info->robustMin(), info->robustMax(), info->maxTime());
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
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
