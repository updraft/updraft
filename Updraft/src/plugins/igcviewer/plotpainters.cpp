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
  int base = axes->getBase();
  QVector<QPolygon> positivePolygons;
  QVector<QPolygon> negativePolygons;

  for (int i = 1; i < buffer.size(); i++) {
    newX = buffer[i].x();
    newY = buffer[i].y();

    if ((newY > base) && (y > base)) {  // both are positive
      QPolygon polygon;
      polygon << QPoint(x, y) << QPoint(newX, newY)
        << QPoint(newX, base) << QPoint(x, base);
      positivePolygons.append(polygon);
    } else {
      if ((newY < base) && (y < base)) {  // both are negative
        QPolygon polygon;
        polygon << QPoint(newX, newY) << QPoint(x, y)
          << QPoint(x, base) << QPoint(newX, base);
        negativePolygons.append(polygon);
      } else {
        if ((newY <= base) && (y >= base)) {
          qreal ratio1 = y - base;
          qreal ratio2 = base - newY;
          if (ratio1 + ratio2 == 0) {
            // if both points lie on the base axis
            x = newX;
            y = newY;
            continue;
          }
          int baseX = x + ratio1 * (((qreal)(newX - x)) / (ratio1 + ratio2));
          QPolygon polygon1;
          polygon1 << QPoint(x, y) << QPoint(baseX, base) << QPoint(x, base);
          QPolygon polygon2;
          polygon2 << QPoint(baseX, base) << QPoint(newX, base)
            << QPoint(newX, newY);
          positivePolygons.append(polygon1);
          negativePolygons.append(polygon2);
        } else {
          qreal ratio1 = base - y;
          qreal ratio2 = newY - base;
          int baseX = x + ratio1 * (((qreal)(newX - x)) / (ratio1 + ratio2));
          QPolygon polygon1;
          polygon1 << QPoint(x, base) << QPoint(baseX, base) << QPoint(x, y);
          QPolygon polygon2;
          polygon2 << QPoint(baseX, base) << QPoint(newX, newY)
            << QPoint(newX, base);
          negativePolygons.append(polygon1);
          positivePolygons.append(polygon2);
        }
      }
    }
    x = newX;
    y = newY;
  }
  painter->setPen(POSITIVE_PEN);
  painter->setBrush(POSITIVE_BRUSH);
  for (int i = 0; i < positivePolygons.size(); i++) {
    painter->drawConvexPolygon(positivePolygons[i]);
  }
  painter->setPen(NEGATIVE_PEN);
  painter->setBrush(NEGATIVE_BRUSH);
  for (int i = 0; i < negativePolygons.size(); i++) {
    painter->drawConvexPolygon(negativePolygons[i]);
  }
  // painter->drawPolyline(buffer);
}

void GroundSpeedPlotPainter::flushBuffer() {
  painter->setPen(QPen(Qt::yellow));
  painter->drawPolyline(buffer);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
