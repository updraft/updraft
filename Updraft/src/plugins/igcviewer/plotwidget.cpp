#include "plotwidget.h"

#include <QDebug>
#include <QPainter>
#include <QGridLayout>
#include <QMouseEvent>

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = QColor(Qt::black);
const QPen PlotWidget::ALTITUDE_PEN = QPen(Qt::red);
const QPen PlotWidget::VERTICAL_SPEED_PEN = QPen(Qt::blue);
const QPen PlotWidget::GROUND_SPEED_PEN= QPen(Qt::yellow);
const QPen PlotWidget::MOUSE_LINE_PEN = QPen(QColor(100, 100, 100));

static const qreal LN10 = qLn(10);

PlotWidget::PlotWidget(IgcInfo* altitudeInfo, IgcInfo* verticalSpeedInfo,
  IgcInfo *groundSpeedInfo)
  : altitudeInfo(altitudeInfo), verticalSpeedInfo(verticalSpeedInfo),
    groundSpeedInfo(groundSpeedInfo) {
    // set mouse tracking
  setMouseTracking(true);
  xLine = -1;
  mouseOver = false;

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

  altitudePlotPainter = new AltitudePlotPainter();
  altitudePlotPainter->init(altitudeAxes, altitudeInfo);

  altitudeLabel = new AxisLabel(altitudeAxes, "m:");
  layout->addItem(altitudeLabel, 0, 0);

  altitudeTimeLabel = new TimeLabel(altitudeAxes);
  layout->addItem(altitudeTimeLabel, 1, 1);

  verticalSpeedAxes = new PlotAxes(false, false);
  verticalSpeedAxes->setLimits(
    verticalSpeedInfo->min(), verticalSpeedInfo->max(), minTime, maxTime);
  layout->addItem(verticalSpeedAxes, 2, 1);

  verticalSpeedPlotPainter = new VerticalSpeedPlotPainter();
  verticalSpeedPlotPainter->init(verticalSpeedAxes, verticalSpeedInfo);

  verticalSpeedLabel = new AxisLabel(verticalSpeedAxes, "m/s:");
  layout->addItem(verticalSpeedLabel, 2, 0);

  groundSpeedAxes = new PlotAxes(false);
  groundSpeedAxes->setLimits(
    groundSpeedInfo->min(), groundSpeedInfo->max(), minTime, maxTime);
  layout->addItem(groundSpeedAxes, 3, 1);

  groundSpeedPlotPainter = new GroundSpeedPlotPainter();
  groundSpeedPlotPainter->init(groundSpeedAxes, groundSpeedInfo);

  groundSpeedLabel = new AxisLabel(groundSpeedAxes, "km/h");
  layout->addItem(groundSpeedLabel, 3, 0);

  // ownership of axes is transfered to layout,
  // ownership of layout is transfered to this.
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), BG_COLOR);

  altitudePlotPainter->draw(&painter);
  altitudeLabel->draw(&painter);
  altitudeTimeLabel->draw(&painter);

  verticalSpeedPlotPainter->draw(&painter);
  verticalSpeedLabel->draw(&painter);

  groundSpeedPlotPainter->draw(&painter);
  groundSpeedLabel->draw(&painter);

  if (mouseOver) {
    painter.setPen(MOUSE_LINE_PEN);
    painter.drawLine(QPoint(xLine, 0), QPoint(xLine, height()));
  }
}

void PlotWidget::mouseMoveEvent(QMouseEvent* mouseEvent) {
  int x = mouseEvent->x();
  QString info;
  if ((x >= altitudePlotPainter->getMinX()) &&
    (x <= altitudePlotPainter->getMaxX())) {
    xLine = x;
    mouseOver = true;
    QString altitude;
    altitude.setNum(altitudePlotPainter->getValueAtPixelX(x), 5, 2);
    QString vspeed;
    vspeed.setNum(verticalSpeedPlotPainter->getValueAtPixelX(x), 5, 2);
    QString gspeed;
    gspeed.setNum(groundSpeedPlotPainter->getValueAtPixelX(x), 5, 2);
    info = "Altitude:\n" + altitude + " m.\n"
      + "Vertical Speed:\n" + vspeed + " m/s.\n"
      + "Ground Speed:\n" + gspeed + " km/h.";
  } else {
    mouseOver = false;
  }

  emit updateInfo(info);
  update();
}

void PlotWidget::leaveEvent(QEvent *e) {
  mouseOver = false;
  QString empty;
  emit updateInfo(empty);
  update();
}

}  // End namespace IgcViewer
}  // End namespace Updraft
