#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTPAINTERS_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTPAINTERS_H_

#include <QPolygonF>
#include <QPointF>

#include "igcinfo.h"
#include "plotaxes.h"

namespace Updraft {
namespace IgcViewer {

/// Value of a pixel in plot.
struct Data {
  int pixel;
  qreal value;
};

/// Abstract class that plots FixInfo to PlotAxes.
/// PlotPainter implements the drawing style.
class PlotPainter : public QObject {
  Q_OBJECT

 public:
  virtual ~PlotPainter() {}
  void init(PlotAxes *axes, FixInfo *info);

  virtual void draw(QPainter* painter);

  /// Returns the index range of the igc info for pixel x.
  virtual void getRangeAtPixel(int x,
    int* startIndex, int* endIndex);

  int getMinX();
  int getMaxX();

 public slots:
  void updateBuffer();

 protected:
  /// Draw points from the buffer.
  /// This method is called every time the graph crosses zero,
  /// at the end of the plot and maybe sometimes more.
  virtual void flushBuffer() = 0;
  virtual void computePoints();
  virtual void computeDrawingData();

  QPainter *painter;
  PlotAxes *axes;
  FixInfo *info;

  QPolygonF buffer;
  QVector<Data> dataValues;
  QVector<int> indexes;
};

/// Painter for plotting altitude.
class AltitudePlotPainter: public PlotPainter {
 protected:
  void flushBuffer();
};

/// Painter for plotting vertical speed.
/// Plots positive values as red solid areas and negative values as
/// blue solid values.
class VerticalSpeedPlotPainter: public PlotPainter {
 protected:
  void flushBuffer();
  void computeDrawingData();
 private:
  QVector<QPolygon> positivePolygons;
  QVector<QPolygon> negativePolygons;

  static const QBrush POSITIVE_BRUSH;
  static const QBrush NEGATIVE_BRUSH;
  static const QPen POSITIVE_PEN;
  static const QPen NEGATIVE_PEN;
};

/// Painter for plotting ground speed.
class GroundSpeedPlotPainter: public PlotPainter {
 protected:
  void flushBuffer();
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOTPAINTERS_H_

