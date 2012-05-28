#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_GRAPHLABELS_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_GRAPHLABELS_H_

#include <QLayoutItem>
#include <QPainter>

namespace Updraft {
namespace IgcViewer {

class PlotAxes;

class Label : public QLayoutItem {
 public:
  /// Overridden from QLayoutItem
  /// \{
  virtual Qt::Orientations expandingDirections() const;
  virtual bool isEmpty() const;
  virtual QRect geometry() const;
  virtual QSize maximumSize() const;
  virtual QSize minimumSize() const;
  virtual QSize sizeHint() const;
  virtual void setGeometry(const QRect& rect);
  /// \}
  virtual void draw(QPainter* painter);
 protected:
  QRect rect;
};

class AxisLabel : public Label {
 public:
  /// Overridden from QLayoutItem
  /// \{
  Qt::Orientations expandingDirections() const;
  QSize maximumSize() const;
  QSize minimumSize() const;
  QSize sizeHint() const;
  /// \}

  /// Draw the labels to the painter.
  void draw(QPainter *painter);

  explicit AxisLabel(PlotAxes* axis, QString unitsDescription);

 private:
  PlotAxes* axis;
  QString unitsDescription;

  static const int MIN_WIDTH = 20;
  static const int MIN_HEIGHT = 20;
  static const int OFFSET_X = 3;
  static const int TEXT_HEIGHT = 10;

  static const QPen LABEL_PEN;
};

class TextLabel : public Label {
 public:
  /// Overridden from QLayoutItem
  /// \{
  Qt::Orientations expandingDirections() const;
  QSize maximumSize() const;
  QSize minimumSize() const;
  QSize sizeHint() const;
  /// \}

  /// Draw the labels to the painter.
  void draw(QPainter *painter);

  explicit TextLabel(QString text_);

 private:
  QString text;

  static const int MIN_WIDTH = 100;
  static const int MIN_HEIGHT = 15;

  static const QPen LABEL_PEN;
};

class TimeLabel : public Label {
 public:
  /// Overridden from QLayoutItem
  /// \{
  Qt::Orientations expandingDirections() const;
  QSize maximumSize() const;
  QSize minimumSize() const;
  QSize sizeHint() const;
  /// \}

  /// Draw the labels to the painter.
  void draw(QPainter *painter);

  explicit TimeLabel(PlotAxes* axis);

 private:
  PlotAxes* axis;

  static const int MIN_WIDTH = 100;
  static const int MIN_HEIGHT = 15;
  static const int TEXT_WIDTH_HEIGHT_RATIO = 4;
  static const int TEXT_MIN_WIDTH = 20;

  static const QPen LABEL_PEN;
};
}
}

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_GRAPHLABELS_H_
