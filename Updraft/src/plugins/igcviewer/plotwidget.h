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

struct PickData {
  PickData(int xLine_, int fixIndex_)
    : xLine(xLine_), fixIndex(fixIndex_) {}

  /// x-coordinate of the pixel in the graph.
  int xLine;

  /// Index of the fix in the fix list.
  int fixIndex;
};

class PlotWidget : public QWidget {
  Q_OBJECT

 public:
  PlotWidget(SegmentInfo* segmentInfo, FixInfo* altitudeInfo,
    FixInfo* verticalSpeedInfo, FixInfo *groundSpeedInfo);

  /// A fix of given index was picked.
  void addPickedFix(int index);

  /// User clicked on position x on the graph, and picked one of the fixes
  /// that is at that position.
  /// Which fix is picked if multiple fixes are on that position is
  /// decided by the method chooseFixIndex(int start, int end);
  void addPickedLine(int x);

  QList<QString>* getSegmentsStatTexts();
  QList<QString>* getPointsStatTexts();

 signals:
  /// Sends text statistics of the fix the mouse is pointing at.
  void updateCurrentInfo(const QString& text);

  /// Send a signal that the vectors with the statistics texts
  /// (for points and segments) has been changed.
  void updateText();

  /// A signal when a user pickes out a fix.
  void fixWasPicked(int index);

  /// A signal upon mouse move over fix.
  void fixIsPointedAt(int index);

  /// A signal when the users clears the picked fixes.
  void clearMarkers();

 private:
  void paintEvent(QPaintEvent* paintEvent);
  void mouseMoveEvent(QMouseEvent* mouseEvent);
  void mousePressEvent(QMouseEvent* mouseEvent);
  void leaveEvent(QEvent* leaveEvent);
  void resizeEvent(QResizeEvent* resizeEvent);

  QSize sizeHint() const;

  void redrawGraphPicture();

  QString createPointStatText(int xLine, int fixListIndex);
  QString createSegmentStatText(int startPointIndex, int endPointIndex);

  /// Update picked texts when a new fix was added at the position `i` in
  /// the `pickedFixes` list.
  void updatePickedTexts(int i);

  /// Resets the statistics to the default version.
  void resetStats();

  /// Create time from secs. The value of secs can be bigger than 24*3600.
  QTime getTimeFromSecs(int timeInSecs);

  /// Pick out which fix of the fixes at given pixel should be picked out.
  int chooseFixIndex(int start, int end);

    /// The coordinate to draw the vertical line where the mouse points.
  int xLine;

  /// The set of picked fixes.
  QList<PickData> pickedFixes;

  /// Ordered list of the x coordinates of the picked positions.
  QList<int> pickedPositions;

  /// Statistics texts for picked fixes.
  QList<QString> segmentsStatTexts;

  /// Statistics texts for segments between picked fixes.
  QList<QString> pickedFixesStatTexts;

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
  FixInfo* altitudeInfo;
  FixInfo* verticalSpeedInfo;
  FixInfo *groundSpeedInfo;

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
