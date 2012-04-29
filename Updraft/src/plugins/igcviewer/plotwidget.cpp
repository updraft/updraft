#include "plotwidget.h"

#include <QDebug>
#include <QPainter>
#include <QGridLayout>
#include <QMouseEvent>
#include <QLabel>

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = QColor(Qt::black);
const QPen PlotWidget::ALTITUDE_PEN = QPen(Qt::red);
const QPen PlotWidget::VERTICAL_SPEED_PEN = QPen(Qt::blue);
const QPen PlotWidget::GROUND_SPEED_PEN= QPen(Qt::yellow);
const QPen PlotWidget::MOUSE_LINE_PEN = QPen(QColor(150, 150, 150));
const QPen PlotWidget::MOUSE_LINE_PICKED_PEN = QPen(QColor(200, 200, 200));

PlotWidget::PlotWidget(IgcInfo* altitudeInfo, IgcInfo* verticalSpeedInfo,
  IgcInfo *groundSpeedInfo)
  : altitudeInfo(altitudeInfo), verticalSpeedInfo(verticalSpeedInfo),
    groundSpeedInfo(groundSpeedInfo) {
    // set mouse tracking
  setMouseTracking(true);
  xLine = -1;
  xLinePicked = -1;
  timePicked = -1;
  mouseOver = false;
  graphPicture = new QImage();

  QGridLayout* layout = new QGridLayout();
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 20);
  layout->setRowStretch(0, 1);
  layout->setRowStretch(1, 10);
  layout->setRowStretch(2, 1);
  layout->setRowStretch(3, 1);
  layout->setRowStretch(4, 10);
  layout->setRowStretch(5, 1);
  layout->setRowStretch(6, 10);

  setLayout(layout);

  qreal minTime = altitudeInfo->absoluteMinTime();
  qreal maxTime = altitudeInfo->absoluteMaxTime();

  TextLabel* altLabel = new TextLabel("Altitude [m]");
  layout->addItem(altLabel, 0, 1);
  labels.append(altLabel);

  altitudeAxes = new PlotAxes();
  altitudeAxes->setLimits(
    altitudeInfo->min(), altitudeInfo->max(), minTime, maxTime);
  layout->addItem(altitudeAxes, 1, 1);

  altitudePlotPainter = new AltitudePlotPainter();
  altitudePlotPainter->init(altitudeAxes, altitudeInfo);

  AxisLabel* altitudeLabel = new AxisLabel(altitudeAxes, "[m]");
  layout->addItem(altitudeLabel, 1, 0);
  labels.append(altitudeLabel);

  TimeLabel* altitudeTimeLabel = new TimeLabel(altitudeAxes);
  layout->addItem(altitudeTimeLabel, 2, 1);
  labels.append(altitudeTimeLabel);

  TextLabel* gsLabel = new TextLabel("Ground speed [km/h]");
  layout->addItem(gsLabel, 3, 1);
  labels.append(gsLabel);

  groundSpeedAxes = new PlotAxes(false);
  groundSpeedAxes->setLimits(
    groundSpeedInfo->min(), groundSpeedInfo->max(), minTime, maxTime);
  layout->addItem(groundSpeedAxes, 4, 1);

  groundSpeedPlotPainter = new GroundSpeedPlotPainter();
  groundSpeedPlotPainter->init(groundSpeedAxes, groundSpeedInfo);

  AxisLabel* groundSpeedLabel = new AxisLabel(groundSpeedAxes, "[km/h]");
  layout->addItem(groundSpeedLabel, 4, 0);
  labels.append(groundSpeedLabel);

  TextLabel* vsLabel = new TextLabel("Vertical speed [m/s]");
  layout->addItem(vsLabel, 5, 1);
  labels.append(vsLabel);

  verticalSpeedAxes = new PlotAxes(false, false);
  verticalSpeedAxes->setLimits(
    verticalSpeedInfo->min(), verticalSpeedInfo->max(), minTime, maxTime);
  layout->addItem(verticalSpeedAxes, 6, 1);

  verticalSpeedPlotPainter = new VerticalSpeedPlotPainter();
  verticalSpeedPlotPainter->init(verticalSpeedAxes, verticalSpeedInfo);

  AxisLabel* verticalSpeedLabel = new AxisLabel(verticalSpeedAxes, "[m/s]");
  layout->addItem(verticalSpeedLabel, 6, 0);
  labels.append(verticalSpeedLabel);

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
    info = getInfoText(x);
  } else {
    mouseOver = false;
  }
  emit updateCurrentInfo(info);
  update();
}

QString PlotWidget::getInfoText(int x) {
  QString info;
  QString hrs;
  QString mins;
  QString secs;
  int timeInSecs = altitudePlotPainter->getTimeAtPixelX(x);
  int timeHrs = timeInSecs / 3600;
  int timeMins = (timeInSecs - timeHrs*3600) / 60;
  mins.setNum(timeMins);
  if (timeMins < 10) mins = "0" + mins;
  int timeSecs = timeInSecs - timeHrs*3600 - timeMins*60;
  secs.setNum(timeSecs);
  if (timeSecs < 10) secs = "0" + secs;
    // if the flight went over the midnight
  if (timeHrs >= 24) timeHrs -= 24;
  hrs.setNum(timeHrs);
  QString altitude;
  altitude.setNum(altitudePlotPainter->getValueAtPixelX(x), 5, 0);
  QString gspeed;
  gspeed.setNum(groundSpeedPlotPainter->getValueAtPixelX(x), 5, 0);
  QString vspeed;
  vspeed.setNum(verticalSpeedPlotPainter->getValueAtPixelX(x), 5, 1);
  info = "Time " + hrs + ":" + mins + ":" + secs + "\n"
    + "Alt " + altitude + " m\n"
    + "GS " + gspeed + " km/h\n"
    + "Vario " + vspeed + " m/s";
  return info;
}

void PlotWidget::mousePressEvent(QMouseEvent* mouseEvent) {
  if (mouseEvent->button() == Qt::LeftButton) {
    int x = mouseEvent->x();
    if ((x >= altitudePlotPainter->getMinX()) &&
      (x <= altitudePlotPainter->getMaxX())) {
      xLinePicked = x;
      timePicked = altitudePlotPainter->getTimeAtPixelX(x);
      emit updatePickedInfo(getInfoText(x));
      update();
    }
  } else {
    if (mouseEvent->button() == Qt::RightButton) {
      xLinePicked = -1;
      timePicked = -1;
      emit updatePickedInfo("");
      update();
    }
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
  verticalSpeedPlotPainter->draw(&painter);
  groundSpeedPlotPainter->draw(&painter);

  for (int i = 0; i < labels.size(); i++) {
    labels[i]->draw(&painter);
  }

  mouseOver = false;

  if (timePicked >= 0) {
    xLinePicked = altitudeAxes->placeX(timePicked);
    emit updatePickedInfo(getInfoText(xLinePicked));
  }
  emit updateCurrentInfo("");
  update();
}

void IGCTextWidget::setMouseOverText(const QString& text) {
  mouseOverText = text;
  updateText();
}

void IGCTextWidget::setPickedText(const QString& text) {
  pickedText = text;
  updateText();
}

void IGCTextWidget::updateText() {
  QString string;
  if (!pickedText.isEmpty()) {
    string = pickedText;
    if (!mouseOverText.isEmpty()) {
      string += "\n\n-----------\n\n" +
        mouseOverText;
    }
  } else {
    string = mouseOverText;
  }
  setText(string);
}

}  // End namespace IgcViewer
}  // End namespace Updraft
