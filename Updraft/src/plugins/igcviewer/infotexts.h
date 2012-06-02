#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_INFOTEXTS_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_INFOTEXTS_H_

#include <QTextEdit>
#include <QTime>
#include "graphlabels.h"

namespace Updraft {
namespace IgcViewer {

struct PickData {
  PickData(int xLine_, QTime time_, int infoIndex_)
    : xLine(xLine_), time(time_), infoIndex(infoIndex_) {}

  /// x-coordinate of the pixel in the graph.
  int xLine;

  /// Time of the picked fix.
  QTime time;

  /// Index of the fix in the fix list.
  int infoIndex;
};

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

  PickedLabel(QList<PickData>* p, QList<QString>* t):
    pickedPositions(p), texts(t) {}

 private:
  QList<PickData>* pickedPositions;
  QList<QString>* texts;

  static const int MIN_WIDTH = 100;
  static const int MIN_HEIGHT = 60;

  static const int TEXT_WIDTH = 100;
  static const int SPACE = 2;

  static const QPen LABEL_PEN;
};

class IGCTextWidget : public QTextEdit {
  Q_OBJECT

 public:
  IGCTextWidget() : segmentsTexts(NULL), pointsTexts(NULL) {}
  IGCTextWidget(QList<QString>* s, QList<QString>* p)
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
