#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_

#include <QColor>
#include <QPen>
#include <QWidget>
#include <QTextEdit>
#include <QTime>

#include "igcinfo.h"
#include "plotaxes.h"
#include "graphlabels.h"
#include "infotexts.h"
#include "plotpainters.h"

namespace Updraft {
namespace IgcViewer {

class PlotWidget : public QWidget {
  Q_OBJECT

 public:
  PlotWidget(SegmentInfo* SegmentInfo, IgcInfo* altitudeInfo,
    IgcInfo* verticalSpeedInfo, IgcInfo *groundSpeedInfo);

  void addPickedTime(QTime time);

  /// sets the line to position x, and returns the time of the pick
  int setPickedLine(int x);

  void addPickedLine(int x);

  QList<QString>* getSegmentsStatTexts();
  QList<QString>* getPointsStatTexts();

 signals:
  void updateCurrentInfo(const QString& text);
  void updateText();
  void timeWasPicked(QTime time);
  void clearMarkers();

 private:
  void paintEvent(QPaintEvent* paintEvent);
  void mouseMoveEvent(QMouseEvent* mouseEvent);
  void mousePressEvent(QMouseEvent* mouseEvent);
  void leaveEvent(QEvent* leaveEvent);
  void resizeEvent(QResizeEvent* resizeEvent);

  void redrawGraphPicture();

  QString createPointStatText(QTime time, int xLine, int fixListIndex);
  QString createSegmentStatText(int startPointIndex, int endPointIndex);
  void updatePickedTexts(int i);
  QTime getTimeFromSecs(int timeInSecs);

    /// The coordinate to draw the vertical line where the mouse points.
  int xLine;

  /// The coordinate to draw the vertical line at a picked location
  /// - when user clicked on the graph.
  QList<PickData> pickedPoints;
  QList<QString> segmentsStatTexts;
  QList<QString> pickedPointsStatTexts;

  QImage* graphPicture;

  QVector<Label*> labels;
  PickedLabel* pickedLabel;

  PlotAxes *altitudeAxes;
  PlotAxes *verticalSpeedAxes;
  PlotAxes *groundSpeedAxes;

  AltitudePlotPainter* altitudePlotPainter;
  VerticalSpeedPlotPainter* verticalSpeedPlotPainter;
  GroundSpeedPlotPainter* groundSpeedPlotPainter;

  SegmentInfo* segmentInfo;
  IgcInfo* altitudeInfo;
  IgcInfo* verticalSpeedInfo;
  IgcInfo *groundSpeedInfo;

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

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
