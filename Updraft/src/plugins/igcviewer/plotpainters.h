#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTPAINTERS_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTPAINTERS_H_

#include <QPolygonF>
#include <QPointF>

#include "igcinfo.h"
#include "plotaxes.h"

namespace Updraft {
namespace IgcViewer {

class PlotPainter {
 public:
  virtual ~PlotPainter() {}
  void init(QPainter *painter, PlotAxes *axes, IgcInfo *info);

  virtual void draw();

  QColor bg;

 protected:
  /// Draw points from the buffer.
  /// This method is called every time the graph crosses zero,
  /// at the end of the plot and maybe sometimes more.
  virtual void flushBuffer() = 0;

  QPainter *painter;
  PlotAxes *axes;
  IgcInfo *info;

  QPolygonF buffer;
};

class AltitudePlotPainter: public PlotPainter {
 protected:
  void flushBuffer();
};

class VerticalSpeedPlotPainter: public PlotPainter {
 protected:
  void flushBuffer();
};

class GroundSpeedPlotPainter: public PlotPainter {
 protected:
  void flushBuffer();
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTPAINTERS_H_

