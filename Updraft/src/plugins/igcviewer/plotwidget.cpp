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
const QPen PlotWidget::MOUSE_LINE_PICKED_PEN = QPen(QColor(120, 120, 120));

static const qreal LN10 = qLn(10);

PlotWidget::PlotWidget(IgcInfo* altitudeInfo, IgcInfo* verticalSpeedInfo,
  IgcInfo *groundSpeedInfo)
  : altitudeInfo(altitudeInfo), verticalSpeedInfo(verticalSpeedInfo),
    groundSpeedInfo(groundSpeedInfo) {
    // set mouse tracking
  setMouseTracking(true);
  xLine = -1;
  xLinePicked = -1;
  mouseOver = false;
  graphPicture = new QImage();

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
  painter.drawImage(0, 0, *graphPicture);
  painter.setRenderHint(QPainter::Antialiasing);

    // draw picked line:
  if (xLinePicked > -1) {
    painter.setPen(MOUSE_LINE_PICKED_PEN);
    painter.drawLine(QPoint(xLinePicked, 0), QPoint(xLinePicked, height()));
  }
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

  emit updateCurrentInfo(info);
  update();
}

void PlotWidget::mousePressEvent(QMouseEvent* mouseEvent) {
  int x = mouseEvent->x();
  QString info;
  if ((x >= altitudePlotPainter->getMinX()) &&
    (x <= altitudePlotPainter->getMaxX())) {
    xLinePicked = x;
    QString altitude;
    altitude.setNum(altitudePlotPainter->getValueAtPixelX(x), 5, 2);
    QString vspeed;
    vspeed.setNum(verticalSpeedPlotPainter->getValueAtPixelX(x), 5, 2);
    QString gspeed;
    gspeed.setNum(groundSpeedPlotPainter->getValueAtPixelX(x), 5, 2);
    info = "Altitude:\n" + altitude + " m.\n"
      + "Vertical Speed:\n" + vspeed + " m/s.\n"
      + "Ground Speed:\n" + gspeed + " km/h.";
    emit updatePickedInfo(info);
    update();
  }
}

void PlotWidget::leaveEvent(QEvent* leaveEvent) {
  mouseOver = false;
  emit updateCurrentInfo("");
  update();
}

void PlotWidget::resizeEvent(QResizeEvent* resizeEvent) {
  delete(graphPicture);  // delete the old pixel map
  graphPicture = new QImage(width(), height(), QImage::Format_RGB32);

    // draw the graph into the pixel map:
  QPainter painter(graphPicture);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), BG_COLOR);

  altitudePlotPainter->draw(&painter);
  altitudeLabel->draw(&painter);
  altitudeTimeLabel->draw(&painter);

  verticalSpeedPlotPainter->draw(&painter);
  verticalSpeedLabel->draw(&painter);

  groundSpeedPlotPainter->draw(&painter);
  groundSpeedLabel->draw(&painter);

  mouseOver = false;
  xLinePicked = -1;
  QString empty;
  emit updateCurrentInfo(empty);
  emit updatePickedInfo(empty);
}

void IGCTextWidget::setMouseOverText(const QString& text) {
  mouseOverText = text;
  QString string = "Picked:\n" + pickedText + "\n\n" +
    "Current:\n" + mouseOverText;
  setText(string);
}

void IGCTextWidget::setPickedText(const QString& text) {
  pickedText = text;
  QString string = "Picked:\n" + pickedText + "\n\n" +
    "Current:\n" + mouseOverText;
  setText(string);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
