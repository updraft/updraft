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

PlotWidget::PlotWidget(SegmentInfo* segmentInfo, FixInfo* altitudeInfo,
  FixInfo* verticalSpeedInfo, FixInfo *groundSpeedInfo)
  :segmentInfo(segmentInfo), altitudeInfo(altitudeInfo),
  verticalSpeedInfo(verticalSpeedInfo), groundSpeedInfo(groundSpeedInfo) {
    // set mouse tracking
  setMouseTracking(true);
  setContextMenuPolicy(Qt::PreventContextMenu);
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

  TextLabel* altLabel = new TextLabel("   " + tr("Altitude") + " [m]");
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

  TextLabel* gsLabel = new TextLabel("   " + tr("Ground speed") + " [km/h]");
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

  TextLabel* vsLabel = new TextLabel("   " + tr("Vertical speed") + " [m/s]");
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

  pickedLabel = new PickedLabel(&pickedPositions, &segmentsStatTexts);
  layout->addItem(pickedLabel, 7, 1);
  labels.append(pickedLabel);

  resetStats();

  // ownership of axes is transfered to layout,
  // ownership of layout is transfered to this.
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  QPainter painter(this);
  painter.drawImage(0, 0, *graphPicture);
  painter.setRenderHint(QPainter::Antialiasing);

    // draw picked line:
  if (!pickedFixes.empty()) {
    painter.setPen(MOUSE_LINE_PICKED_PEN);
    for (int i = 0; i < pickedFixes.size(); i++) {
      painter.drawLine(QPoint(pickedFixes[i].xLine, 0),
        QPoint(pickedFixes[i].xLine, pickedLabel->geometry().top()));
    }
  }
  if (mouseOver) {
    painter.setPen(MOUSE_LINE_PEN);
    painter.drawLine(QPoint(xLine, 0),
      QPoint(xLine, pickedLabel->geometry().top()));
  }
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

void PlotWidget::leaveEvent(QEvent* leaveEvent) {
  mouseOver = false;
  emit updateCurrentInfo("");
  emit fixIsPointedAt(-1);
  update();
}

void PlotWidget::resizeEvent(QResizeEvent* resizeEvent) {
  for (int i = 0; i < pickedFixes.size(); i++) {
    qreal secs = altitudeInfo->absoluteTime(pickedFixes[i].fixIndex);
    int x = altitudeAxes->placeX(secs);
    pickedFixes[i].xLine = x;
    pickedPositions[i] = x;
  }
  redrawGraphPicture();
  updateText();
}

void PlotWidget::mouseMoveEvent(QMouseEvent* mouseEvent) {
  int x = mouseEvent->x();
  QString info;
  if ((x >= altitudePlotPainter->getMinX()) &&
    (x <= altitudePlotPainter->getMaxX())) {
    xLine = x;

    mouseOver = true;
    int startIndex, endIndex;
    altitudePlotPainter->getRangeAtPixel(x, &startIndex, &endIndex);
      // we take the first info item that has fallen into the pixel
    int index = chooseFixIndex(startIndex, endIndex);
    info = createPointStatText(x, index);
    emit fixIsPointedAt(index);
  } else {
    mouseOver = false;
    emit fixIsPointedAt(-1);
  }
  emit updateCurrentInfo(info);
  update();
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
      resetStats();
      emit clearMarkers();
      update();
    }
  }
}

void PlotWidget::addPickedLine(int x) {
  int startIndex, endIndex;
  altitudePlotPainter->getRangeAtPixel(x, &startIndex, &endIndex);

    // we always take the first value of the range for the pixel
    // and forget about the rest
  int index = chooseFixIndex(startIndex, endIndex);

  int i;
  for (i = 0; i < pickedFixes.count(); i++) {
    if (pickedFixes[i].fixIndex == index) return;  // ignore this click
    if (pickedFixes[i].fixIndex > index) break;
  }
  pickedFixes.insert(i, PickData(x, index));
  pickedPositions.insert(i, x);
  updatePickedTexts(i);

  emit updateText();
  emit fixWasPicked(index);
  update();
}

void PlotWidget::addPickedFix(int index) {
  int timeInSecs = altitudeInfo->absoluteTime(index);

  int x = altitudeAxes->placeX(timeInSecs);

  int i;
  for (i = 0; i < pickedFixes.count(); i++) {
    if (pickedFixes[i].fixIndex == index) return;  // ignore this click
    if (pickedFixes[i].fixIndex > index) break;
  }
  pickedFixes.insert(i, PickData(x, index));
  pickedPositions.insert(i, x);
  updatePickedTexts(i);

  emit updateText();
  update();
}

void PlotWidget::updatePickedTexts(int i) {
  int c = pickedFixes.size();
  int a = pickedFixes[0].xLine;
  int aa = pickedFixes[1].xLine;
  int aaa = pickedFixes[2].xLine;

  QString prevStat = createSegmentStatText(
    pickedFixes[i-1].fixIndex, pickedFixes[i].fixIndex);
  QString nextStat = createSegmentStatText(
    pickedFixes[i].fixIndex, pickedFixes[i+1].fixIndex);

  segmentsStatTexts.removeAt(i-1);

  segmentsStatTexts.insert(i-1, prevStat);
  segmentsStatTexts.insert(i, nextStat);

  int index = pickedFixes[i].fixIndex;
  int x = pickedFixes[i].xLine;
  pickedFixesStatTexts.insert(i, createPointStatText(x, index));
  redrawGraphPicture();
}

void PlotWidget::resetStats() {
  pickedFixesStatTexts.clear();
  pickedPositions.clear();
  pickedFixes.clear();
  segmentsStatTexts.clear();

  PickData first(qMax(0, altitudePlotPainter->getMinX()), 0);
  PickData last(qMax(0, altitudePlotPainter->getMaxX()),
    altitudeInfo->count()-1);

  pickedPositions.append(first.xLine);
  pickedPositions.append(last.xLine);

  pickedFixes.append(first);
  pickedFixes.append(last);

  pickedFixesStatTexts.append(createPointStatText(first.xLine, first.fixIndex));
  pickedFixesStatTexts.append(createPointStatText(last.xLine, last.fixIndex));

  segmentsStatTexts.append(
    createSegmentStatText(first.fixIndex, last.fixIndex));

  updateText();
  redrawGraphPicture();
}

QString PlotWidget::createPointStatText(int xLine, int index) {
  QTime time = altitudeInfo->timestamp(index);

  QString info;
  QString hrs;
  QString mins;
  QString secs;

  hrs.setNum(time.hour());
  mins.setNum(time.minute());
  if (time.minute() < 10) mins = "0" + mins;
  secs.setNum(time.second());
  if (time.second() < 10) secs = "0" + secs;

  QString altitude;
  altitude.setNum(altitudeInfo->value(index), 5, 0);
  QString gspeed;
  gspeed.setNum(groundSpeedInfo->value(index), 5, 0);
  QString vspeed;
  vspeed.setNum(verticalSpeedInfo->value(index), 5, 1);
  info = tr("Time") + " " + hrs + ":" + mins + ":" + secs + "\n"
    + tr("Alt") + " " + altitude + " m\n"
    + tr("GS") + " " + gspeed + " km/h\n"
    + tr("Vario") + " " + vspeed + " m/s";
  return info;
}

QString PlotWidget::createSegmentStatText(
  int startPointIndex, int endPointIndex) {
  QString text;
  QTime startTime = segmentInfo->timestamp(startPointIndex);
  QTime endTime = segmentInfo->timestamp(endPointIndex);
  qreal distance = segmentInfo->distance(startPointIndex, endPointIndex);
  qreal avgSpeed = segmentInfo->avgSpeed(startPointIndex, endPointIndex);
  qreal avgRise = segmentInfo->avgRise(startPointIndex, endPointIndex);
  qreal heightDiff =
    segmentInfo->heightDifference(startPointIndex, endPointIndex);

    // fill the text
  QString durationhrs;
  QString durationmins;
  QString durationsecs;

  int durationSecs = startTime.secsTo(endTime);
  if (durationSecs < 0) durationSecs += 24 * 3600;

  QTime durationTime = getTimeFromSecs(durationSecs);

  durationhrs.setNum(durationTime.hour());
  durationmins.setNum(durationTime.minute());
  if (durationTime.minute() < 10) durationmins = "0" + durationmins;
  durationsecs.setNum(durationTime.second());
  if (durationTime.second() < 10) durationsecs = "0" + durationsecs;

  QString distancestr;
  distancestr.setNum(distance/1000.0, 1, 0);
  QString heightstr;
  heightstr.setNum(heightDiff, 5, 0);
  QString avgspeedstr;
  avgspeedstr.setNum(avgSpeed, 5, 0);
  QString avgrisestr;
  avgrisestr.setNum(avgRise, 5, 1);
  text = "dT: "
    + durationhrs + ":" + durationmins + ":" + durationsecs + "\n"
    + "dS: " + distancestr + " km\n"
    + "dH: " + heightstr + " m\n"
    + "GS: " + avgspeedstr + " km/h\n"
    + "VS: " + avgrisestr + " m/s";
  return text;
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

int PlotWidget::chooseFixIndex(int start, int end) {
  return start;
}

QList<QString>* PlotWidget::getSegmentsStatTexts() {
  return &segmentsStatTexts;
}

QList<QString>* PlotWidget::getPointsStatTexts() {
  return &pickedFixesStatTexts;
}

}  // End namespace IgcViewer
}  // End namespace Updraft
