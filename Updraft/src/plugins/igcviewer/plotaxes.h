#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_

#include <QPoint>
#include <QPainter>
#include <QLayoutItem>
#include <QLinkedList>

#include "util/util.h"

namespace Updraft {
namespace IgcViewer {

/// X and Y axis for graph plotting.
class PlotAxes : public QObject, public QLayoutItem {
  Q_OBJECT

 public:
  /// Overridden from QLayoutItem
  /// \{
  Qt::Orientations expandingDirections() const;
  bool isEmpty() const;
  QRect geometry() const;
  QSize maximumSize() const;
  QSize minimumSize() const;
  QSize sizeHint() const;
  void setGeometry(const QRect& rect);
  /// \}

  explicit PlotAxes(bool drawTimeTicks = true, bool drawAxisX = true);

  /// Get the X coordinate for drawing.
  qreal placeX(qreal x);

  /// Get the inverse value for "placeX"
  qreal getInverseX(qreal x);

  /// Get the Y coordinate for drawing.
  qreal placeY(qreal y);

  /// Get the inverse value for "placeY"
  qreal getInverseY(qreal y);

  /// Set the limits for drawing and recalculate all cached values.
  void setLimits(qreal min, qreal max, qreal minTime, qreal maxTime);

  /// Draw the axes to the painter
  void draw(QPainter *painter);

  /// Returns the Y coordinate of base axis: the Y coordinate of value 0.
  qreal getBase();

  /// Returns the value of the vertical step.
  qreal getStep();

  /// Returns the value of the first vertical tick.
  qreal getFirst();

  /// Returns the value of time interval in seconds.
  int getTimeInterval();

  /// Returns the value of the time start in seconds.
  qreal getStartTimeTick();

  /// Returns the number of the vertical ticks.
  int getVerticalTicks();

    /// Returns the value of the time start in seconds.
  int getFirstHour();

  /// Returns the number of the vertical ticks.
  int getLastHour();

 signals:
  void geometryChanged();

 private:
  /// Signal change of geometry.
  void emitGeometryChanged();

  /// Return tick value increment in vertical axis.
  qreal findTickIncrement(qreal range, qreal width, qreal minTickSpacing);

  /// Return tick value increment in vertical axis.
  qreal findTimeTickIncrement(qreal range, qreal width, qreal minTickSpacing);

  /// Linear functions for placing points.
  Util::LinearFunc linX, linY;

  Util::LinearFunc tickX, tickY;
  int tickCountX, tickCountY;

  QRect rect;
  qreal min, max, maxTime, minTime;

  /// Draw time ticks.
  bool drawTimeTicks;

  /// Whether to draw the X axis.
  bool drawAxisX;

  /// Position (in drawing cordinates) of the X base.
  qreal base;

  /// A step in vertical axis.
  qreal step;

  /// The value of the first vertical tick.
  qreal first;

  /// The step in time axis in seconds.
  int timeInterval;

  /// The start time in seconds.
  int startTimeTick;

  /// First and last hour for labels.
  int firstHour, lastHour;

  /// Smallest distance between ticks.
  static const int TICK_SPACING_X = 10;
  static const int TICK_SPACING_Y = 10;
  static const int TIME_TICK_HEIGHT = 10;

  static const QPen AXES_PEN;
  static const QPen TICKS_PEN;
  static const QPen TIME_TICKS_PEN;

  /// Predefined time interval values.
  QVector<int> timeIntervalValues;
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_
