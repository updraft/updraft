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
  PlotWidget(IgcInfo* altitudeInfo, IgcInfo* verticalSpeedInfo,
    IgcInfo *groundSpeedInfo);

 private:
  void paintEvent(QPaintEvent* paintEvent);

  PlotAxes *altitudeAxes;
  PlotAxes *verticalSpeedAxes;
  PlotAxes *groundSpeedAxes;

  TimeLabel* altitudeTimeLabel;

  AxisLabel* altitudeLabel;
  AxisLabel* verticalSpeedLabel;
  AxisLabel* groundSpeedLabel;

  IgcInfo* altitudeInfo;
  IgcInfo* verticalSpeedInfo;
  IgcInfo *groundSpeedInfo;

  /// Offset from the window border
  static const int OFFSET_X = 15;
  static const int OFFSET_Y = 10;

  static const QColor BG_COLOR;
  static const QPen ALTITUDE_PEN;
  static const QPen VERTICAL_SPEED_PEN;
  static const QPen GROUND_SPEED_PEN;
};

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTWIDGET_H_
