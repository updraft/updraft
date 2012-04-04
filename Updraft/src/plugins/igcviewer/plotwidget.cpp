#include "plotwidget.h"

#include <QDebug>
#include <QPainter>
// #include <QVBoxLayout>
#include <QGridLayout>

#include "plotpainters.h"

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = QColor(Qt::black);
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
  layout->setColumnStretch(1, 20);
  layout->setRowStretch(0, 7);
  layout->setRowStretch(1, 1);
  layout->setRowStretch(2, 7);
  layout->setRowStretch(3, 7);

  setLayout(layout);

  qreal minTime = altitudeInfo->absoluteMinTime();
  qreal maxTime = altitudeInfo->absoluteMaxTime();

  altitudeAxes = new PlotAxes();
  altitudeAxes->setLimits(
    altitudeInfo->min(), altitudeInfo->max(), minTime, maxTime);
  layout->addItem(altitudeAxes, 0, 1);

  altitudeLabel = new AxisLabel(altitudeAxes, "m:");
  layout->addItem(altitudeLabel, 0, 0);

  altitudeTimeLabel = new TimeLabel(altitudeAxes);
  layout->addItem(altitudeTimeLabel, 1, 1);

  verticalSpeedAxes = new PlotAxes(false, false);
  verticalSpeedAxes->setLimits(
    verticalSpeedInfo->min(), verticalSpeedInfo->max(), minTime, maxTime);
  layout->addItem(verticalSpeedAxes, 2, 1);

  verticalSpeedLabel = new AxisLabel(verticalSpeedAxes, "m/s:");
  layout->addItem(verticalSpeedLabel, 2, 0);

  groundSpeedAxes = new PlotAxes(false);
  groundSpeedAxes->setLimits(
    groundSpeedInfo->min(), groundSpeedInfo->max(), minTime, maxTime);
  layout->addItem(groundSpeedAxes, 3, 1);

  groundSpeedLabel = new AxisLabel(groundSpeedAxes, "km/h");
  layout->addItem(groundSpeedLabel, 3, 0);

  // ownership of axes is transfered to layout,
  // ownership of layout is transfered to this.
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), BG_COLOR);

  AltitudePlotPainter altitudePainter;
  altitudePainter.init(&painter, altitudeAxes, altitudeInfo);
  altitudePainter.draw();
  altitudeLabel->draw(&painter);
  altitudeTimeLabel->draw(&painter);

  VerticalSpeedPlotPainter verticalSpeedPainter;
  verticalSpeedPainter.init(&painter, verticalSpeedAxes, verticalSpeedInfo);
  verticalSpeedPainter.draw();
  verticalSpeedLabel->draw(&painter);

  GroundSpeedPlotPainter groundSpeedPainter;
  groundSpeedPainter.init(&painter, groundSpeedAxes, groundSpeedInfo);
  groundSpeedPainter.draw();
  groundSpeedLabel->draw(&painter);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
