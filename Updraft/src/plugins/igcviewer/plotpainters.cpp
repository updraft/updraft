#include "plotpainters.h"
#include <QDebug>

namespace Updraft {
namespace IgcViewer {

const QBrush VerticalSpeedPlotPainter::POSITIVE_BRUSH = QBrush(Qt::red);
const QPen VerticalSpeedPlotPainter::POSITIVE_PEN = QPen(Qt::red);
const QBrush VerticalSpeedPlotPainter::NEGATIVE_BRUSH = QBrush(Qt::blue);
const QPen VerticalSpeedPlotPainter::NEGATIVE_PEN = QPen(Qt::blue);

void PlotPainter::init(QPainter *painter, PlotAxes *axes, IgcInfo *info) {
  this->painter = painter;
  this->axes = axes;
  this->info = info;
}

void PlotPainter::draw() {
  int count = 1;
  int x = qFloor(axes->placeX(info->absoluteTime(0)));
  qreal sum = axes->placeY(info->value(0));

  axes->draw(painter);

  for (int i = 1; i < info->count(); ++i) {
    int newX = qFloor(axes->placeX(info->absoluteTime(i)));

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
  if (buffer.empty()) return;
  int x = buffer[0].x();
  int y = buffer[0].y();
  int newX = x;
  int newY = y;
  int base = axes->placeY(0);
  int b = axes->getBase();
  qDebug() << base << b;
  QPolygon polygon;

  polygon << QPoint(x, base) << QPoint(x, y);

  for (int i = 1; i < buffer.size(); i++) {
    newX = buffer[i].x();
    newY = buffer[i].y();

    if (((newY > base) && (y > base)) ||
      ((newY < base) && (y < base)) ||
      ((newY == base) && (y == base))) {  // both are at the same side
      polygon << QPoint(newX, newY);  // just append the new point
    } else {
      int ratio1 = qAbs(y-base);
      int ratio2 = qAbs(newY-base);
      int baseIsecX = x + ratio1*((qreal)(newX-x) / (ratio1+ratio2));
      if ((y <= base) && (newY >= base)) {  // we ended the negative part
        polygon << QPoint(baseIsecX, base);
        painter->setBrush(NEGATIVE_BRUSH);
        painter->setPen(NEGATIVE_PEN);
        painter->drawPolygon(polygon);
          // clear the polygon, and start a new one
        polygon.clear();
        polygon << QPoint(baseIsecX+1, base) << QPoint(newX, newY);
      } else {  // we ended the positive part
        polygon << QPoint(baseIsecX, base);
        painter->setBrush(POSITIVE_BRUSH);
        painter->setPen(POSITIVE_PEN);
        painter->drawPolygon(polygon);
          // clear the polygon, and start a new one
        polygon.clear();
        polygon << QPoint(baseIsecX+1, base) << QPoint(newX, newY);
      }
    }
    x = newX;
    y = newY;
  }
  // painter->drawPolyline(buffer);
}

void GroundSpeedPlotPainter::flushBuffer() {
  painter->setPen(QPen(Qt::yellow));
  painter->drawPolyline(buffer);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
