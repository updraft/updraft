#include "plotwidget.h"

#include <QDebug>
#include <QPainter>
// #include <QVBoxLayout>
#include <QGridLayout>

#include "plotpainters.h"

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = Qt::black;
const QPen PlotWidget::ALTITUDE_PEN = QPen(Qt::red);
const QPen PlotWidget::VERTICAL_SPEED_PEN = QPen(Qt::blue);
const QPen PlotWidget::GROUND_SPEED_PEN= QPen(Qt::yellow);

static const qreal LN10 = qLn(10);

PlotWidget::PlotWidget(IgcInfo* altitudeInfo, IgcInfo* verticalSpeedInfo,
  IgcInfo *groundSpeedInfo)
  : altitudeInfo(altitudeInfo), verticalSpeedInfo(verticalSpeedInfo),
    groundSpeedInfo(groundSpeedInfo) {
  // QVBoxLayout* layout = new QVBoxLayout();
  QGridLayout* layout = new QGridLayout();
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 10);

  setLayout(layout);

  qreal maxTime = altitudeInfo->maxTime();

  altitudeAxes = new PlotAxes();
  altitudeAxes->setLimits(
    altitudeInfo->min(), altitudeInfo->max(), maxTime);
  layout->addItem(altitudeAxes, 0, 1);

  altitudeLabel = new AxisLabel(altitudeAxes);
  altitudeLabel->bg = QColor(Qt::blue);
  layout->addItem(altitudeLabel, 0, 0);

  verticalSpeedAxes = new PlotAxes();
  verticalSpeedAxes->setLimits(
    verticalSpeedInfo->min(), verticalSpeedInfo->max(), maxTime);
  layout->addItem(verticalSpeedAxes, 1, 1);

  verticalSpeedLabel = new AxisLabel(verticalSpeedAxes);
  verticalSpeedLabel->bg = QColor(Qt::green);
  layout->addItem(verticalSpeedLabel, 1, 0);

  groundSpeedAxes = new PlotAxes();
  groundSpeedAxes->setLimits(
    groundSpeedInfo->min(), groundSpeedInfo->max(), maxTime);
  layout->addItem(groundSpeedAxes, 2, 1);

  groundSpeedLabel = new AxisLabel(groundSpeedAxes);
  groundSpeedLabel->bg = QColor(Qt::red);
  layout->addItem(groundSpeedLabel, 2, 0);

  // ownership of axes is transfered to layout,
  // ownership of layout is transfered to this.

  /*
  layout->setStretch(0, 2);
  layout->setStretch(1, 1);
  layout->setStretch(2, 1);
  */
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), BG_COLOR);

  AltitudePlotPainter altitudePainter;
  altitudePainter.init(&painter, altitudeAxes, altitudeInfo);
  altitudePainter.bg = QColor(100, 100, 100);
  altitudePainter.draw();
  altitudeLabel->draw(&painter);

  VerticalSpeedPlotPainter verticalSpeedPainter;
  verticalSpeedPainter.init(&painter, verticalSpeedAxes, verticalSpeedInfo);
  verticalSpeedPainter.bg = QColor(255, 0, 255);
  verticalSpeedPainter.draw();
  verticalSpeedLabel->draw(&painter);

  GroundSpeedPlotPainter groundSpeedPainter;
  groundSpeedPainter.init(&painter, groundSpeedAxes, groundSpeedInfo);
  groundSpeedPainter.bg = QColor(255, 255, 255);
  groundSpeedPainter.draw();
  groundSpeedLabel->draw(&painter);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
