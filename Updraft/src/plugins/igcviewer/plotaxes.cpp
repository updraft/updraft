#include "plotaxes.h"

#include <QtCore/qmath.h>
#include <QDebug>
#include <QLayout>

namespace Updraft {
namespace IgcViewer {

const QPen PlotAxes::AXES_PEN = QPen(QBrush(Qt::gray), 2);
const QPen PlotAxes::TICKS_PEN = QPen(QBrush(Qt::gray), 0, Qt::DotLine);
const QPen PlotAxes::TIME_TICKS_PEN = QPen(QBrush(Qt::gray), 1);
const QPen AxisLabel::LABEL_PEN = QPen(Qt::white);
const QPen TimeLabel::LABEL_PEN = QPen(Qt::white);

static const qreal LN10 = qLn(10);

PlotAxes::PlotAxes(bool drawTimeTicks_, bool drawAxisX_) {
  this->drawTimeTicks = drawTimeTicks_;
  this->drawAxisX = drawAxisX_;
  // fill the predefined time interval values
  // the values are in seconds
  timeIntervalValues.append(30);
  timeIntervalValues.append(60);
  timeIntervalValues.append(300);
  timeIntervalValues.append(600);
  timeIntervalValues.append(900);
  timeIntervalValues.append(1800);
  timeIntervalValues.append(3600);
  timeIntervalValues.append(2*3600);
  timeIntervalValues.append(4*3600);
  timeIntervalValues.append(8*3600);
  timeIntervalValues.append(16*3600);
}

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

  linX.set(minTime, rect.left(), maxTime, rect.right());
  linY.set(min, rect.bottom(), max, rect.top());

  qreal tick;

  /// time axis:
  tick = findTimeTickIncrement(maxTime - minTime, rect.width(), TICK_SPACING_X);
  this->timeInterval = tick;
  tickCountX = qFloor((maxTime-minTime) / tick) + 1;
  this->startTimeTick = tick * qCeil(minTime / tick);
  tickX = linX.compose(Util::LinearFunc(tick, startTimeTick));

  /// vertical axis:
  tick = findTickIncrement(max - min, rect.height(), TICK_SPACING_Y);
  this->first = tick * qCeil(min / tick);
  tickCountY = qFloor((max - first) / tick) + 1;
  tickY = linY.compose(Util::LinearFunc(tick, first));
  this->step = tick;

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

void PlotAxes::setLimits(qreal min, qreal max, qreal minTime, qreal maxTime) {
  this->min = min;
  this->max = max;
  this->minTime = minTime;
  this->maxTime = maxTime;
  this->firstHour = qCeil(minTime / 3600);
  this->lastHour = qFloor(maxTime / 3600);

  setGeometry(rect);
}

qreal PlotAxes::findTickIncrement(qreal range, qreal size,
  qreal minTickSpacing) {
  qreal tmp = minTickSpacing * range / size;

  qreal logIncrement = qCeil(qLn(tmp) / LN10);
  qreal increment = qExp(logIncrement * LN10);

  qreal pxPerTick = increment * size / range;

  if (pxPerTick > 5 * minTickSpacing) {
    return increment / 5;
  } else if (pxPerTick > 2 * minTickSpacing) {
    return increment / 2;
  } else {
    return increment;
  }
}

qreal PlotAxes::findTimeTickIncrement
(qreal range, qreal size, qreal minTickSpacing) {
    // this is when the rectangle is not initiated yet
  if (size == 0) return 0;
  qreal tmp = minTickSpacing * range / size;
  int interval = 0;

  // find the suiting increment
  int index;
  for (index = 0; index < timeIntervalValues.size(); index++) {
    if (timeIntervalValues[index] >= tmp) break;
  }

  if (index == timeIntervalValues.size()) {
    qDebug() << "No value" << tmp;
    interval = 0;
  } else {
    interval = timeIntervalValues[index];
  }

  return interval;
}

void PlotAxes::draw(QPainter *painter) {
  // Ticks
  painter->setPen(TICKS_PEN);

  if (drawTimeTicks) {
    for (int i = 0; i < tickCountX; ++i) {
      qreal pixelX = tickX.get(i);

      painter->drawLine(QPointF(pixelX, base - TIME_TICK_HEIGHT/2),
        QPointF(pixelX, base + TIME_TICK_HEIGHT/2));
    }

    // draw the whole hour ticks
    for (int hour = firstHour; hour <= lastHour; ++hour) {
      qreal pixelX = placeX(hour*3600);

      painter->drawLine(QPointF(pixelX, rect.bottom()),
        QPointF(pixelX, rect.top()));
    }
  }

  for (int i = 0; i < tickCountY; ++i) {
    qreal pixelY = tickY.get(i);

    painter->drawLine(QPointF(rect.left(), pixelY),
      QPointF(rect.right(), pixelY));
  }

  // Axes
  painter->setPen(AXES_PEN);

  painter->drawLine(QPointF(rect.topLeft()), QPointF(rect.bottomLeft()));

  if (drawAxisX) {
    painter->drawLine(QPointF(rect.left(), base), QPointF(rect.right(), base));
  }
}

qreal PlotAxes::getBase() {
  return base;
}

qreal PlotAxes::getStep() {
  return step;
}

qreal PlotAxes::getFirst() {
  return first;
}

int PlotAxes::getTimeInterval() {
  return timeInterval;
}

qreal PlotAxes::getStartTimeTick() {
  return startTimeTick;
}

int PlotAxes::getFirstHour() {
  return firstHour;
}

int PlotAxes::getLastHour() {
  return lastHour;
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
  return QSize(50, 65536);
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

AxisLabel::AxisLabel(PlotAxes* axis_, QString unitsDescription_) {
  unitsDescription = unitsDescription_;
  axis = axis_;
}

void AxisLabel::draw(QPainter *painter) {
  painter->setPen(LABEL_PEN);
  qreal step = axis->getStep();
  qreal first = axis->getFirst();
  int nLabels = axis->getVerticalTicks();
  for (int i = 0; i < nLabels; ++i) {
    qreal pixelY = axis->placeY(first+step*i);

    QString text;
    text.setNum(first+step*i);
    QRect labelRect(QPoint(0, pixelY-TEXT_HEIGHT/2),
      QPoint(rect.right(), pixelY+TEXT_HEIGHT/2));
    painter->drawText(labelRect, Qt::AlignRight, text);
  }

  // draw the units desription at the position
  // of the last label:
  qreal lastTick = nLabels-1;
  qreal pixelY = axis->placeY(first+step*lastTick);

  QString text(unitsDescription);
  QRect labelRect(QPoint(2, pixelY-TEXT_HEIGHT/2),
    QPoint(rect.right(), pixelY+TEXT_HEIGHT/2));
  // painter->drawText(labelRect, Qt::AlignLeft, text);
}

// Time Labels:

Qt::Orientations TimeLabel::expandingDirections() const {
  return Qt::Horizontal;
}

QRect TimeLabel::geometry() const {
  return rect;
}

bool TimeLabel::isEmpty() const {
  return false;
}

QSize TimeLabel::maximumSize() const {
  return QSize(65536, 65536);
}

QSize TimeLabel::minimumSize() const {
  return QSize(MIN_WIDTH, MIN_HEIGHT);
}

QSize TimeLabel::sizeHint() const {
  return QSize(100, 100);
}

void TimeLabel::setGeometry(const QRect& rect_) {
  this->rect = rect_;
}

TimeLabel::TimeLabel(PlotAxes* axis_) {
  axis = axis_;
}

void TimeLabel::draw(QPainter* painter) {
  painter->setPen(LABEL_PEN);
  int height = rect.height();
  int width = height * TEXT_WIDTH_HEIGHT_RATIO;
    // don't draw the labels if there is no space
  if (width < TEXT_MIN_WIDTH) return;

  int firstHour = axis->getFirstHour();
  int lastHour = axis->getLastHour();
  int min = qMin(lastHour, 23);
  QString zeros(":00");

  for (int hour = firstHour; hour <= min; ++hour) {
    qreal pixelX = axis->placeX(hour * 3600);

    QString hrsText;
    hrsText.setNum(hour);
    QRect labelRect(QPoint(pixelX - width/2, rect.top()),
      QPoint(pixelX - 2, rect.bottom()));
    painter->drawText(labelRect, Qt::AlignRight, hrsText);
    QRect zerosRect(QPoint(pixelX-1, rect.top()),
      QPoint(pixelX + width/2, rect.bottom()));
    painter->drawText(zerosRect, Qt::AlignLeft, zeros);
  }


  for (int hour = 24; hour <= lastHour; ++hour) {
    qreal pixelX = axis->placeX(hour * 3600);

    QString hrsText;
    hrsText.setNum(hour - 24);
    QRect labelRect(QPoint(pixelX - width/2, rect.top()),
      QPoint(pixelX - 2, rect.bottom()));
    painter->drawText(labelRect, Qt::AlignRight, hrsText);
    QRect zerosRect(QPoint(pixelX-1, rect.top()),
      QPoint(pixelX + width/2, rect.bottom()));
    painter->drawText(zerosRect, Qt::AlignLeft, zeros);
  }
}

}  // End namespace IgcViewer
}  // End namespace Updraft
