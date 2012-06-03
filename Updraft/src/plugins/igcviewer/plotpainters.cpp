#include "plotpainters.h"
#include <QDebug>

namespace Updraft {
namespace IgcViewer {

const QBrush VerticalSpeedPlotPainter::POSITIVE_BRUSH =
  QBrush(QColor(100, 100, 255));
const QPen VerticalSpeedPlotPainter::POSITIVE_PEN = QPen(QColor(100, 100, 255));
const QBrush VerticalSpeedPlotPainter::NEGATIVE_BRUSH = QBrush(Qt::red);
const QPen VerticalSpeedPlotPainter::NEGATIVE_PEN = QPen(Qt::red);

void PlotPainter::init(PlotAxes *axes, FixInfo *info) {
  this->axes = axes;
  this->info = info;

  connect(axes, SIGNAL(geometryChanged()), this, SLOT(updateBuffer()));
}

void PlotPainter::draw(QPainter* painter) {
  this->painter = painter;
  axes->draw(painter);
  flushBuffer();
}

int PlotPainter::getMinX() {
  return dataValues.first().pixel;
}

int PlotPainter::getMaxX() {
  return dataValues.last().pixel;
}

void PlotPainter::updateBuffer() {
  computePoints();
  computeDrawingData();
}

void PlotPainter::computePoints() {
  buffer.clear();
  dataValues.clear();
  indexes.clear();
  int count = 1;
  int x = qFloor(axes->placeX(info->absoluteTime(0)));
  qreal sum = axes->placeY(info->value(0));
  qreal dataSum = info->value(0);
  indexes.append(0);

  for (int i = 1; i < info->count(); ++i) {
    int newX = qFloor(axes->placeX(info->absoluteTime(i)));

    if (newX != x) {
      buffer.append(QPointF(x + 0.5, sum / count));
      for (int k = 0; k < newX - x; k++) {
        indexes.append(i);
      }
      Data d = {x, dataSum / count};
      dataValues.append(d);
      count = 0;
      x = newX;
      sum = 0;
      dataSum = 0;
    }

    count += 1;
    sum += axes->placeY(info->value(i));
    dataSum += info->value(i);
  }

  buffer.append(QPointF(x + 0.5, sum / count));
  Data d = {x, dataSum / count};
  dataValues.append(d);
  // don't append to the indexes array: there are the starting indexes.
}

void PlotPainter::getRangeAtPixel(int x,
  int* startIndex, int* endIndex) {
  int startPixel = axes->placeX(info->absoluteTime(0));
  int i = x - startPixel;

  *startIndex = *endIndex = -1;
  if ((i < 0) || (i > indexes.size())) return;
  if (i < indexes.size() - 1) {
    *startIndex = indexes[i];
    *endIndex = indexes[i+1];
  } else {  // if it's the last pixel
    *startIndex = indexes.last();
    *endIndex = info->count();
  }
}

void PlotPainter::computeDrawingData() {
  // do nothing, everything is already in the buffer.
}

void AltitudePlotPainter::flushBuffer() {
  painter->setPen(QPen(Qt::red));
  painter->drawPolyline(buffer);
}

void VerticalSpeedPlotPainter::computeDrawingData() {
    // clear the polygons lists
  positivePolygons.clear();
  negativePolygons.clear();

  if (buffer.empty()) return;
  int x = buffer[0].x();
  int y = buffer[0].y();
  int newX = x;
  int newY = y;
  int base = axes->placeY(0);
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
        negativePolygons.append(polygon);
          // clear the polygon, and start a new one
        polygon.clear();
        polygon << QPoint(baseIsecX+1, base) << QPoint(newX, newY);
      } else {  // we ended the positive part
        polygon << QPoint(baseIsecX, base);
        positivePolygons.append(polygon);
          // clear the polygon, and start a new one
        polygon.clear();
        polygon << QPoint(baseIsecX+1, base) << QPoint(newX, newY);
      }
    }
    x = newX;
    y = newY;
  }
}

void VerticalSpeedPlotPainter::flushBuffer() {
  painter->setBrush(POSITIVE_BRUSH);
  painter->setPen(POSITIVE_PEN);
  for (int i = 0; i < positivePolygons.size(); i++) {
    painter->drawPolygon(positivePolygons[i]);
  }
  painter->setBrush(NEGATIVE_BRUSH);
  painter->setPen(NEGATIVE_PEN);
  for (int i = 0; i < negativePolygons.size(); i++) {
    painter->drawPolygon(negativePolygons[i]);
  }
}

void GroundSpeedPlotPainter::flushBuffer() {
  painter->setPen(QPen(Qt::yellow));
  painter->drawPolyline(buffer);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
