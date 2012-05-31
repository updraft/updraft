#include "plotwidget.h"

#include <QDebug>
#include <QPainter>
#include <QGridLayout>
#include <QMouseEvent>
#include <QLabel>
#include <QTime>

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = QColor(Qt::black);
const QPen PlotWidget::ALTITUDE_PEN = QPen(Qt::red);
const QPen PlotWidget::VERTICAL_SPEED_PEN = QPen(Qt::blue);
const QPen PlotWidget::GROUND_SPEED_PEN= QPen(Qt::yellow);
const QPen PlotWidget::MOUSE_LINE_PEN = QPen(QColor(150, 150, 150));
const QPen PlotWidget::MOUSE_LINE_PICKED_PEN = QPen(QColor(200, 200, 200));

PlotWidget::PlotWidget(TrackData* trackData, IgcInfo* altitudeInfo,
  IgcInfo* verticalSpeedInfo, IgcInfo *groundSpeedInfo)
  : trackData(trackData), altitudeInfo(altitudeInfo),
  verticalSpeedInfo(verticalSpeedInfo), groundSpeedInfo(groundSpeedInfo) {
    // set mouse tracking
  setMouseTracking(true);
  xLine = -1;
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
  layout->setRowStretch(7, 1);

  setLayout(layout);

  qreal minTime = altitudeInfo->absoluteMinTime();
  qreal maxTime = altitudeInfo->absoluteMaxTime();

  TextLabel* altLabel = new TextLabel(tr("Altitude") + " [m]");
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

  TextLabel* gsLabel = new TextLabel(tr("Ground speed") + " [km/h]");
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

  TextLabel* vsLabel = new TextLabel(tr("Vertical speed") + " [m/s]");
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

  pickedLabel = new PickedLabel(&pickedPoints, &segmentsStatTexts);
  layout->addItem(pickedLabel, 7, 1);
  labels.append(pickedLabel);

  segmentsStatTexts.append(createSegmentStatText(-1, 1));
  // ownership of axes is transfered to layout,
  // ownership of layout is transfered to this.
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  QPainter painter(this);
  painter.drawImage(0, 0, *graphPicture);
  painter.setRenderHint(QPainter::Antialiasing);

    // draw picked line:
  if (!pickedPoints.empty()) {
    painter.setPen(MOUSE_LINE_PICKED_PEN);
    for (int i = 0; i < pickedPoints.size(); i++) {
      painter.drawLine(QPoint(pickedPoints[i].xLine, 0),
        QPoint(pickedPoints[i].xLine, pickedLabel->geometry().top()));
    }
  }
  if (mouseOver) {
    painter.setPen(MOUSE_LINE_PEN);
    painter.drawLine(QPoint(xLine, 0),
      QPoint(xLine, pickedLabel->geometry().top()));
  }
}

void PlotWidget::mouseMoveEvent(QMouseEvent* mouseEvent) {
  int x = mouseEvent->x();
  QString info;
  if ((x >= altitudePlotPainter->getMinX()) &&
    (x <= altitudePlotPainter->getMaxX())) {
    xLine = x;

    mouseOver = true;
    int secs = altitudePlotPainter->getTimeAtPixelX(x);
    info = createPointStatText(getTimeFromSecs(secs), x);
  } else {
    mouseOver = false;
  }
  emit updateCurrentInfo(info);
  update();
}

QString PlotWidget::createPointStatText(QTime time, int xLine) {
  QString info;
  QString hrs;
  QString mins;
  QString secs;

  hrs.setNum(time.hour());
  mins.setNum(time.minute());
  if (time.minute() < 10) mins = "0" + mins;
  secs.setNum(time.second());
  if (time.second() < 10) secs = "0" + secs;

  int x = xLine;

  QString altitude;
  altitude.setNum(altitudePlotPainter->getValueAtPixelX(x), 5, 0);
  QString gspeed;
  gspeed.setNum(groundSpeedPlotPainter->getValueAtPixelX(x), 5, 0);
  QString vspeed;
  vspeed.setNum(verticalSpeedPlotPainter->getValueAtPixelX(x), 5, 1);
  info = tr("Time") + " " + hrs + ":" + mins + ":" + secs + "\n"
    + tr("Alt") + " " + altitude + " m\n"
    + tr("GS") + " " + gspeed + " km/h\n"
    + tr("Vario") + " " + vspeed + " m/s";
  return info;
}

void PlotWidget::mousePressEvent(QMouseEvent* mouseEvent) {
  if (mouseEvent->button() == Qt::LeftButton) {
    int x = mouseEvent->x();
    if ((x >= altitudePlotPainter->getMinX()) &&
      (x <= altitudePlotPainter->getMaxX())) {
      addPickedLine(x);
    }
  } else {
    if (mouseEvent->button() == Qt::RightButton) {
      pickedPoints.clear();
      segmentsStatTexts.clear();
      segmentsStatTexts.append(createSegmentStatText(-1, 1));
      redrawGraphPicture();
      emit updateText();
      emit clearMarkers();
      update();
    }
  }
}

void PlotWidget::leaveEvent(QEvent* leaveEvent) {
  mouseOver = false;
  emit updateCurrentInfo("");
  update();
}

void PlotWidget::redrawGraphPicture() {
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

  emit updateCurrentInfo("");
  update();
}

void PlotWidget::resizeEvent(QResizeEvent* resizeEvent) {
  redrawGraphPicture();
}

QTime PlotWidget::getTimeFromSecs(int timeInSecs) {
  int timeHrs = timeInSecs / 3600;
  int timeMins = (timeInSecs - timeHrs*3600) / 60;
  int timeSecs = timeInSecs - timeHrs*3600 - timeMins*60;
    // if the flight went over the midnight
  if (timeHrs >= 24) timeHrs -= 24;
  QTime timestamp(timeHrs, timeMins, timeSecs);
  return timestamp;
}

void PlotWidget::addPickedLine(int x) {
  int timeInSecs = altitudePlotPainter->getTimeAtPixelX(x);

  QTime timestamp = getTimeFromSecs(timeInSecs);

  int i;
  for (i = 0; i < pickedPoints.count(); i++) {
    if (pickedPoints[i].xLine > x) break;
  }
  pickedPoints.insert(i, PickData(x, timestamp));
  updatePickedTexts(i);

  emit updateText();
  emit timeWasPicked(timestamp);
  update();
}

void PlotWidget::addPickedTime(QTime time) {
  int timeInSecs = time.hour() * 3600 + time.minute() * 60 + time.second();

  int x = altitudeAxes->placeX(timeInSecs);

  int i;
  for (i = 0; i < pickedPoints.count(); i++) {
    if (pickedPoints[i].xLine > x) break;
  }
  pickedPoints.insert(i, PickData(x, time));
  updatePickedTexts(i);

  emit updateText();
  update();
}

void PlotWidget::updatePickedTexts(int i) {
  QString prevStat = createSegmentStatText(i-1, i);
  QString nextStat = createSegmentStatText(i, i+1);

  segmentsStatTexts.removeAt(i);

  segmentsStatTexts.insert(i, prevStat);
  segmentsStatTexts.insert(i+1, nextStat);

  QTime time = pickedPoints[i].time;
  int x = pickedPoints[i].xLine;
  pickedPointsStatTexts.insert(i, createPointStatText(time, x));
  redrawGraphPicture();
}

QString PlotWidget::createSegmentStatText(
  int startPointIndex, int endPointIndex) {
  QString text;
  QTime startTime;
  QTime endTime;
  qreal distance;
  qreal avgSpeed;
  qreal avgRise;
  if ((startPointIndex < 0) && (endPointIndex >= pickedPoints.size())) {
    startTime = trackData->getStartTime();
    endTime = trackData->getEndTime();
    distance = trackData->distanceOverall();
    avgSpeed = trackData->avgSpeedOverall();
    avgRise = trackData->avgRiseOverall();
  } else {
    if (startPointIndex < 0) {
      startTime = trackData->getStartTime();
      endTime = pickedPoints[endPointIndex].time;
      distance = trackData->distanceUntil(endTime);
      avgSpeed = trackData->avgSpeedUntil(endTime);
      avgRise = trackData->avgRiseUntil(endTime);
    } else {
      if (endPointIndex >= pickedPoints.size()) {
        startTime = pickedPoints[startPointIndex].time;
        endTime = trackData->getEndTime();
        distance = trackData->distanceSince(startTime);
        avgSpeed = trackData->avgSpeedSince(startTime);
        avgRise = trackData->avgRiseSince(startTime);
      } else {
        startTime = pickedPoints[startPointIndex].time;
        endTime = pickedPoints[endPointIndex].time;
        distance = trackData->distance(startTime, endTime);
        avgSpeed = trackData->avgSpeed(startTime, endTime);
        avgRise = trackData->avgRise(startTime, endTime);
      }
    }
  }
    // fill the text
  QString starthrs;
  QString startmins;
  QString startsecs;
  QString endhrs;
  QString endmins;
  QString endsecs;

  starthrs.setNum(startTime.hour());
  startmins.setNum(startTime.minute());
  if (startTime.minute() < 10) startmins = "0" + startmins;
  startsecs.setNum(startTime.second());
  if (startTime.second() < 10) startsecs = "0" + startsecs;

  endhrs.setNum(endTime.hour());
  endmins.setNum(endTime.minute());
  if (endTime.minute() < 10) endmins = "0" + endmins;
  endsecs.setNum(endTime.second());
  if (endTime.second() < 10) endsecs = "0" + endsecs;

  QString distancestr;
  distancestr.setNum(distance, 5, 0);
  QString avgspeedstr;
  avgspeedstr.setNum(avgSpeed, 5, 0);
  QString avgrisestr;
  avgrisestr.setNum(avgRise, 5, 1);
  text = starthrs + ":" + startmins + ":" + startsecs
    + " - " + endhrs + ":" + endmins + ":" + endsecs + "\n"
    + distancestr + " m\n"
    + tr("avg GS") + " " + avgspeedstr + " km/h\n"
    + tr("avg VS") + " " + avgrisestr + " m/s";
  return text;
}

QList<QString>* PlotWidget::getSegmentsStatTexts() {
  return &segmentsStatTexts;
}

QList<QString>* PlotWidget::getPointsStatTexts() {
  return &pickedPointsStatTexts;
}

}  // End namespace IgcViewer
}  // End namespace Updraft
