#include "plotpainters.h"

namespace Updraft {
namespace IgcViewer {

void PlotPainter::init(QPainter *painter, PlotAxes *axes, IgcInfo *info) {
  this->painter = painter;
  this->axes = axes;
  this->info = info;
}

void PlotPainter::draw() {
  painter->fillRect(axes->geometry(), bg);
  int count = 1;
  int x = qFloor(axes->placeX(info->time(0)));
  qreal sum = axes->placeY(info->value(0));

  axes->draw(painter);

  for (int i = 1; i < info->count(); ++i) {
    int newX = qFloor(axes->placeX(info->time(i)));

    if (newX != x) {
      buffer.append(QPointF(x + 0.5, sum / count));
      count = 0;
      x = newX;
      sum = 0;
    }

    count += 1;
    sum += axes->placeY(info->value(i));
  }

  buffer.append(QPointF(x + 0.5, sum / count));
  flushBuffer();
  buffer.clear();
}

void AltitudePlotPainter::flushBuffer() {
  painter->setPen(QPen(Qt::red));
  painter->drawPolyline(buffer);
}

void VerticalSpeedPlotPainter::flushBuffer() {
  painter->setPen(QPen(Qt::blue));
  painter->drawPolyline(buffer);
}

void GroundSpeedPlotPainter::flushBuffer() {
  painter->setPen(QPen(Qt::yellow));
  painter->drawPolyline(buffer);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
