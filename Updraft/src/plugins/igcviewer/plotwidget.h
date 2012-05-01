#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_

#include <QColor>
#include <QPen>
#include <QWidget>
#include <QTextEdit>

#include "igcinfo.h"
#include "plotaxes.h"
#include "plotpainters.h"

namespace Updraft {
namespace IgcViewer {

class PlotWidget : public QWidget {
  Q_OBJECT

 public:
  PlotWidget(IgcInfo* altitudeInfo, IgcInfo* verticalSpeedInfo,
    IgcInfo *groundSpeedInfo);

  /// sets the line to the time, and returns the x position of the line
  int setPickedTime(int time);

  /// sets the line to position x, and returns the time of the pick
  int setPickedLine(int x);

 signals:
  void updateCurrentInfo(const QString& text);
  void updatePickedInfo(const QString& text);
  void timeWasPicked(QTime time);
  void displayMarker(bool value);

 private:
  void paintEvent(QPaintEvent* paintEvent);
  void mouseMoveEvent(QMouseEvent* mouseEvent);
  void mousePressEvent(QMouseEvent* mouseEvent);
  void leaveEvent(QEvent* leaveEvent);
  void resizeEvent(QResizeEvent* resizeEvent);

  QString getInfoText(int x);

  QImage* graphPicture;

  QVector<Label*> labels;

  PlotAxes *altitudeAxes;
  PlotAxes *verticalSpeedAxes;
  PlotAxes *groundSpeedAxes;

  AltitudePlotPainter* altitudePlotPainter;
  VerticalSpeedPlotPainter* verticalSpeedPlotPainter;
  GroundSpeedPlotPainter* groundSpeedPlotPainter;

  IgcInfo* altitudeInfo;
  IgcInfo* verticalSpeedInfo;
  IgcInfo *groundSpeedInfo;

  /// The coordinate to draw the vertical line where the mouse points.
  int xLine;

  /// Time [in seconds] of the picked point.
  int timePicked;

  /// The coordinate to draw the vertical line at a picked location
  /// - when user clicked on the graph.
  int xLinePicked;

  /// Whether the mouse if over the graph(!) - not widget.
  bool mouseOver;

  /// Offset from the window border
  static const int OFFSET_X = 15;
  static const int OFFSET_Y = 10;

  static const QColor BG_COLOR;
  static const QPen ALTITUDE_PEN;
  static const QPen VERTICAL_SPEED_PEN;
  static const QPen GROUND_SPEED_PEN;

  static const QPen MOUSE_LINE_PEN;
  static const QPen MOUSE_LINE_PICKED_PEN;
};

class IGCTextWidget : public QTextEdit {
  Q_OBJECT

 public slots:
  void setPickedText(const QString& text);
  void setMouseOverText(const QString& text);

 private:
  QString pickedText;
  QString mouseOverText;
  void updateText();
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
