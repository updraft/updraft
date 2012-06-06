#include "infotexts.h"

#include <QDebug>
#include "plotaxes.h"

namespace Updraft {
namespace IgcViewer {

const QPen PickedLabel::LABEL_PEN = QPen(Qt::white);

  // Picked Label:
Qt::Orientations PickedLabel::expandingDirections() const {
  return Qt::Horizontal;
}

QSize PickedLabel::maximumSize() const {
  return QSize(65536, 65536);
}

QSize PickedLabel::sizeHint() const {
  return QSize(100, -1);
}

QSize PickedLabel::minimumSize() const {
  return QSize(MIN_WIDTH, MIN_HEIGHT);
}

void PickedLabel::draw(QPainter *painter) {
  if (texts->empty()) return;
  if (pickedPositions->empty()) return;

  QFont font("Helvetica", 8);
  painter->setPen(LABEL_PEN);
  painter->setFont(font);

  QVector<int> space;
  int spos = pickedPositions->at(0);
  int epos = 0;
  for (int i = 0; i < pickedPositions->size()-1; i++) {
    epos = pickedPositions->at(i+1);
    space.append(epos - spos);
    spos = epos;
  }

  spos = pickedPositions->at(0);
  for (int i = 0; i < pickedPositions->size()-1; i++) {
    epos = pickedPositions->at(i+1);
    int cspos = spos;
    spos = epos;
    if ((epos - cspos) < TEXT_WIDTH) {
      int res = (TEXT_WIDTH - (epos - cspos)) / 2 - SPACE;
      if (i < 1) continue;
      if (i == pickedPositions->size() - 2) continue;
      if (((space[i-1]-TEXT_WIDTH) / 2) < res) continue;
      if (((space[i+1]-TEXT_WIDTH) / 2) < res) continue;
    }
    int center = (epos - cspos)/2 + cspos;

    QRect textbox(QPoint(center-TEXT_WIDTH/2, rect.top()),
      QPoint(center+TEXT_WIDTH/2, rect.bottom()));
    painter->drawText(textbox, Qt::AlignHCenter, texts->at(i));
  }
}

// IGC Text Info Widget

QSize IgcTextWidget::sizeHint() const {
  return QSize(100, -1);
}

void IgcTextWidget::setMouseOverText(const QString& text) {
  mouseOverText = text;
  updateText();
}

void IgcTextWidget::setSegmentsTexts(QList<QString>* text) {
  segmentsTexts = text;
  updateText();
}

void IgcTextWidget::setPointsTexts(QList<QString>* text) {
  pointsTexts = text;
  updateText();
}

void IgcTextWidget::updateText() {
  QFont font("Helvetica", 8);
  setFont(font);
  QString string;
  QString s;
  if (!mouseOverText.isEmpty()) {
    s = mouseOverText;
    s.replace("\n", "<br>");
    string += "<b>" + s + "</b><br>";
    string += "-----------<br>";
  }
  // qAssert(segmentsTexts->size() == pointsTexts->size()-1);
  for (int i = 0; i < pointsTexts->size()-1; i++) {
    s = pointsTexts->at(i);
    s.replace("\n", "<br>");
    string += "<b>" + s + "</b><br>";
    s = segmentsTexts->at(i);
    s.replace("\n", "<br>");
    string += "<i>" + s + "</i><br>";
  }
  if (!pointsTexts->isEmpty()) {
    s = pointsTexts->last();
    s.replace("\n", "<br>");
    string += "<b>" + s + "</b><br>";
  }
  setHtml(string);
}
}
}
