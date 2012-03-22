#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_

#include <QWidget>
#include <QPen>
#include <QColor>

#include "igcinfo.h"

namespace Updraft {
namespace IgcViewer {

class PlotWidget : public QWidget {
  Q_OBJECT

 public:
  PlotWidget(IgcInfo* info);

 private:
  /// Return tick value increment.
  int findTickIncrement(qreal range, qreal width, qreal minTickSpacing);

  void paintEvent(QPaintEvent* paintEvent);

  /// Calculate X position of a point
  qreal placeX(qreal val);

  /// Calculate Y position of a point.
  qreal placeY(qreal val);

  IgcInfo* info;

  /// Offset from the window border
  static const int PIXEL_OFFSET_X = 15;
  static const int PIXEL_OFFSET_Y = 10;

  /// Smallest distance between ticks.
  static const int MIN_TICK_SIZE = 10;

  static const QColor BG_COLOR;
  static const QPen AXES_PEN;
  static const QPen TICKS_PEN;
  static const QPen PLOT_PEN;
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
