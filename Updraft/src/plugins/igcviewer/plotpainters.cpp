#include "plotpainters.h"
#include <QDebug>

namespace Updraft {
namespace IgcViewer {

const QBrush VerticalSpeedPlotPainter::POSITIVE_BRUSH =
  QBrush(QColor(100, 100, 255));
const QPen VerticalSpeedPlotPainter::POSITIVE_PEN = QPen(QColor(100, 100, 255));
const QBrush VerticalSpeedPlotPainter::NEGATIVE_BRUSH = QBrush(Qt::red);
const QPen VerticalSpeedPlotPainter::NEGATIVE_PEN = QPen(Qt::red);

void PlotPainter::init(PlotAxes *axes, IgcInfo *info) {
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
  qDebug() << dataValues.last().pixel << " "
    << buffer.last().x();
  return dataValues.last().pixel;
}

void PlotPainter::updateBuffer() {
  computePoints();
  computeDrawingData();
  qDebug() << "Data updated";
}

void PlotPainter::computePoints() {
  buffer.clear();
  dataValues.clear();
  int count = 1;
  int x = qFloor(axes->placeX(info->absoluteTime(0)));
  qreal sum = axes->placeY(info->value(0));
  qreal dataSum = info->value(0);

  for (int i = 1; i < info->count(); ++i) {
    int newX = qFloor(axes->placeX(info->absoluteTime(i)));

    if (newX != x) {
      buffer.append(QPointF(x + 0.5, sum / count));
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
}

qreal PlotPainter::getValueAtPixelX(int x) {
    // deal with extreme cases:
  if (dataValues.empty()) return 0;
  if (x < dataValues.first().pixel) return 0;
  qDebug() << dataValues.last().pixel;
  if (x > dataValues.last().pixel) return 0;

    // guess the position: this should be most of the times
    // the correct guess
  int begin = dataValues[0].pixel;
  int guess = x - begin;
  int position;
  if ((guess >= 0) && (guess < dataValues.size())) {
    position = guess;
  } else {
    position = dataValues.size()/2;
  }

  bool found = (dataValues[position].pixel == x);
  if (found) {
    return dataValues[position].value;
  } else {
    // do binary search for the data with the value of the pixel
    // we are looking for
    int leftIndex = 0;
    int rightIndex = dataValues.size();
    while (true) {
      if (leftIndex == rightIndex) {
        found = false;
        break;
      }
      if (x == dataValues[position].pixel) {
        found = true;
        break;
      }
      if (x < dataValues[position].pixel) {
        rightIndex = position - 1;
      }
      if (x > dataValues[position].pixel) {
        leftIndex = position + 1;
      }
      position = leftIndex + (rightIndex - leftIndex) / 2;
    }
    if (found) {
      return dataValues[position].value;
    } else {
      // interpolate from the neighbours:
      if ((position == 0) || (position == (dataValues.size() - 1)))
        return dataValues[position].value;
      if (x < dataValues[position].pixel) {
        return (dataValues[position-1].value +
          ((x-dataValues[position-1].pixel) /
            (dataValues[position].pixel - dataValues[position-1].pixel))
          * dataValues[position-1].value);
      }
      if (x > dataValues[position].pixel) {
        return (dataValues[position].value +
          ((x-dataValues[position].pixel) /
            (dataValues[position+1].pixel - dataValues[position].pixel))
          * dataValues[position].value);
      }
    }
  }
  return 0;
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
  int b = axes->getBase();
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
