#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_

#include <QColor>
#include <QPen>
#include <QWidget>

#include "igcinfo.h"
#include "plotaxes.h"

namespace Updraft {
namespace IgcViewer {

class PlotWidget : public QWidget {
  Q_OBJECT

 public:
  PlotWidget(IgcInfo* info);

 private:
  void resizeEvent(QResizeEvent* resizeEvent);
  void paintEvent(QPaintEvent* paintEvent);

  PlotAxes axes;

  IgcInfo* info;

  /// Offset from the window border
  static const int OFFSET_X = 15;
  static const int OFFSET_Y = 10;

  static const QColor BG_COLOR;
  static const QPen PLOT_PEN;
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
