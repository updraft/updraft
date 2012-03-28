#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_

#include <QPoint>
#include <QPainter>
#include <QLayoutItem>

#include "util/util.h"

namespace Updraft {
namespace IgcViewer {

/// X and Y axis for graph plotting.
class PlotAxes : public QLayoutItem {
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

  /// Get the X coordinate for drawing.
  qreal placeX(qreal x);

  /// Get the Y coordinate for drawing.
  qreal placeY(qreal y);

  /// Set the limits for drawing and recalculate all cached values.
  void setLimits(qreal min, qreal max, qreal maxTime);

  /// Draw the axes to the painter
  void draw(QPainter *painter);

 private:
  /// Return tick value increment.
  int findTickIncrement(qreal range, qreal width, qreal minTickSpacing);

  /// Linear functions for placing points.
  Util::LinearFunc linX, linY;

  Util::LinearFunc tickX, tickY;
  int tickCountX, tickCountY;

  QRect rect;
  qreal min, max, maxTime;

  /// Position (in drawing cordinates) of the X base.
  qreal base;

  /// Smallest distance between ticks.
  static const int TICK_SPACING_X = 10;
  static const int TICK_SPACING_Y = 10;

  static const QPen AXES_PEN;
  static const QPen TICKS_PEN;
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_