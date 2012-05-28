#include "graphlabels.h"
#include "plotaxes.h"

namespace Updraft {
namespace IgcViewer {

const QPen AxisLabel::LABEL_PEN = QPen(Qt::white);
const QPen TimeLabel::LABEL_PEN = QPen(Qt::white);
const QPen TextLabel::LABEL_PEN = QPen(Qt::white);

  // Labels:
Qt::Orientations Label::expandingDirections() const {
  return Qt::Vertical | Qt::Horizontal;
}

QRect Label::geometry() const {
  return rect;
}

bool Label::isEmpty() const {
  return false;
}

QSize Label::maximumSize() const {
  return QSize(65536, 65536);
}

QSize Label::minimumSize() const {
  return QSize(0, 0);
}

QSize Label::sizeHint() const {
  return QSize(100, 100);
}

void Label::setGeometry(const QRect& rect_) {
  this->rect = rect_;
}

void Label::draw(QPainter *painter) {
  // do nothing.
}

// Axis Labels:

Qt::Orientations AxisLabel::expandingDirections() const {
  return Qt::Vertical;
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
  // qreal lastTick = nLabels-1;
  /*
  qreal pixelY = axis->placeY(first);

  QString text(unitsDescription);
  QRect labelRect(QPoint(2, pixelY-TEXT_HEIGHT/2),
    QPoint(rect.right(), pixelY+TEXT_HEIGHT/2));
  painter->drawText(labelRect, Qt::AlignLeft, text);
  */
}

// Text Label:

Qt::Orientations TextLabel::expandingDirections() const {
  return Qt::Horizontal;
}

QSize TextLabel::maximumSize() const {
  return QSize(65536, 65536);
}

QSize TextLabel::minimumSize() const {
  return QSize(MIN_WIDTH, MIN_HEIGHT);
}

QSize TextLabel::sizeHint() const {
  return QSize(100, 100);
}

TextLabel::TextLabel(QString text_) {
  text = text_;
}

void TextLabel::draw(QPainter* painter) {
  painter->setPen(LABEL_PEN);
  painter->drawText(rect, Qt::AlignLeft | Qt::AlignBottom, text);
}

// Time Label:

Qt::Orientations TimeLabel::expandingDirections() const {
  return Qt::Horizontal;
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
}
}
