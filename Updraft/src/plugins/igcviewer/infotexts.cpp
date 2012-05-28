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
  return QSize(100, 100);
}

QSize PickedLabel::minimumSize() const {
  return QSize(MIN_WIDTH, MIN_HEIGHT);
}

void PickedLabel::draw(QPainter *painter) {
  if (texts->empty()) return;
  QFont font("Helvetica", 8);
  painter->setPen(LABEL_PEN);
  painter->setFont(font);
  int spos = rect.left();
  int epos = rect.right();
  QVector<int> space;
  for (int i = 0; i < pickedPositions->size(); i++) {
    epos = pickedPositions->at(i).xLine;
    space.append(epos - spos);
    spos = epos;
  }
  space.append(rect.right() - spos);

  spos = rect.left();
  for (int i = 0; i < pickedPositions->size(); i++) {
    epos = pickedPositions->at(i).xLine;
    int cspos = spos;
    spos = epos;
    if ((epos - cspos) < TEXT_WIDTH) {
      int res = (TEXT_WIDTH - (epos - cspos)) / 2 - SPACE;
      if (i < 1) continue;
      if (((space[i-1]-TEXT_WIDTH) / 2) < res) continue;
      if (((space[i+1]-TEXT_WIDTH) / 2) < res) continue;
    }
    int center = (epos - cspos)/2 + cspos;

    QRect textbox(QPoint(center-TEXT_WIDTH/2, rect.top()),
      QPoint(center+TEXT_WIDTH/2, rect.bottom()));
    painter->drawText(textbox, texts->at(i));
  }

  // draw the last text, if there is space
  epos = rect.right();
  int res = (TEXT_WIDTH - (epos - spos)) / 2 - SPACE;
  if (((epos - spos) < TEXT_WIDTH) &&
    (space.empty()) || (((space.last() - TEXT_WIDTH) / 2) < res))
    return;

  int center = (epos - spos)/2 + spos;

  QRect textbox(QPoint(center-TEXT_WIDTH/2, rect.top()),
    QPoint(center+TEXT_WIDTH/2, rect.bottom()));
  painter->drawText(textbox, texts->last());
}

// IGC Text Info Widget

Qt::Orientations IGCTextWidget::expandingDirections() const {
  return Qt::Vertical;
}

QSize IGCTextWidget::sizeHint() const {
  return QSize(100, 1000);
}

void IGCTextWidget::setMouseOverText(const QString& text) {
  mouseOverText = text;
  updateText();
}

void IGCTextWidget::setSegmentsTexts(QList<QString>* text) {
  segmentsTexts = text;
  updateText();
}

void IGCTextWidget::setPointsTexts(QList<QString>* text) {
  pointsTexts = text;
  updateText();
}

void IGCTextWidget::updateText() {
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
  for (int i = 0; i < segmentsTexts->size()-1; i++) {
    s = segmentsTexts->at(i);
    s.replace("\n", "<br>");
    string += "<i>" + s + "</i><br>";
    s = pointsTexts->at(i);
    s.replace("\n", "<br>");
    string += "<b>" + s + "</b><br>";
  }
  if (!segmentsTexts->isEmpty()) {
    s = segmentsTexts->last();
    s.replace("\n", "<br>");
    string += "<i>" + s + "</i>";
  }
  setHtml(string);
}
}
}
