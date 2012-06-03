#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_INFOTEXTS_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_INFOTEXTS_H_

#include <QTextEdit>
#include <QTime>
#include "graphlabels.h"

namespace Updraft {
namespace IgcViewer {

class PickedLabel : public Label {
 public:
  /// Overridden from QLayoutItem
  /// \{
  Qt::Orientations expandingDirections() const;
  QSize maximumSize() const;
  QSize minimumSize() const;
  QSize sizeHint() const;
  /// \}
  void draw(QPainter* painter);

  PickedLabel(QList<int>* p, QList<QString>* t):
    pickedPositions(p), texts(t) {}

 private:
  QList<int>* pickedPositions;
  QList<QString>* texts;

  static const int MIN_WIDTH = 100;
  static const int MIN_HEIGHT = 60;

  static const int TEXT_WIDTH = 100;
  static const int SPACE = 2;

  static const QPen LABEL_PEN;
};

class IgcTextWidget : public QTextEdit {
  Q_OBJECT

 public:
  IgcTextWidget() : segmentsTexts(NULL), pointsTexts(NULL) {}
  IgcTextWidget(QList<QString>* s, QList<QString>* p)
    : segmentsTexts(s), pointsTexts(p) {updateText();}

  Qt::Orientations expandingDirections() const;
  QSize sizeHint() const;

 public slots:
  void setSegmentsTexts(QList<QString>* texts);
  void setPointsTexts(QList<QString>* texts);
  void setMouseOverText(const QString& text);
  void updateText();

 private:
  QString mouseOverText;
  QList<QString>* segmentsTexts;
  QList<QString>* pointsTexts;
};
}
}

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_INFOTEXTS_H_
