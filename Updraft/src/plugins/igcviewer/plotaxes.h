#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_

#include <QPoint>
#include <QPainter>

#include "util/util.h"

namespace Updraft {
namespace IgcViewer {

class PlotAxes {
 public:
  /// Calculate a drawing position of a point.
  QPointF placePoint(qreal x, qreal y);
  
  /// Set the limits for drawing
  void setLimits(const QRectF& rect, qreal min, qreal max, qreal maxTime);

  /// Draw the axes to the painter
  void draw(QPainter *painter);

 private:
  /// Return tick value increment.
  int findTickIncrement(qreal range, qreal width, qreal minTickSpacing);

  /// Linear functions for placing points.
  Util::LinearFunc linX, linY;

  Util::LinearFunc tickX, tickY;
  int tickCountX, tickCountY;

  QRectF rect;

  /// Position (in drawing cordinates) of the X base.
  qreal base;

  /// Smallest distance between ticks.
  static const int TICK_SPACING_X = 10;
  static const int TICK_SPACING_Y = 15;

  static const QPen AXES_PEN;
  static const QPen TICKS_PEN;
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTAXES_H_
