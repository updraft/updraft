#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOT_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOT_H_

#include <QWidget>

namespace Updraft {
namespace IgcViewer {

class PlotWidget : public QWidget {
 private:
  void paintEvent(QPaintEvent* paintEvent);

  /// Intervals in which to draw a mark in the time axis.
  QList<int> timeIntervals;

  /// Minimum number of pixels between the marks.
  int minPixels;

  /// offset from the window border
  static const int pixelOffsetX = 5;
  static const int pixelOffsetY = 5;

  /// drawing options
  QColor background;
  QColor axisColor;
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOT_H_
