#include "igcinfo.h"

#include <QtAlgorithms>
#include <QtCore>

#include "igcviewer.h"

namespace Updraft {
namespace IgcViewer {

/// How large part of the range shold be skiped as outliers.
/// \todo Right now the value is just a wild guess. Measure it.
qreal OUTLIERS_SKIP_RANGE = 0.03;

void IgcInfo::init(const QList<TrackFix> *fixList) {
  this->fixList = fixList;

  if (fixList->count() < 1) {
    min_ = max_ = robustMin_ = robustMax_ = 0;
    resetGlobalScale();
    return;
  }

  QList<qreal> values;
  for (int i = 1; i < fixList->count(); ++i) {
    values.append(this->value(i));
  }
  qSort(values);

  int skipCount = qRound(values.count() * OUTLIERS_SKIP_RANGE);

  min_ = values[0];
  max_ = values[values.count() - 1];

  qDebug() << values[values.count() - 1] << " " << values[values.count() - 2]
    << " " << values[values.count() - 3] << " " << values[values.count() - 4];

  robustMin_ = values[skipCount];
  robustMax_ = values[values.count() - 1 - skipCount];

  resetGlobalScale();
}

qreal IgcInfo::time(int i) const {
  if (i == 0) {
    return 0;
  }

  qreal ret = fixList->at(0).timestamp.secsTo(fixList->at(i).timestamp);

  if (ret < 0) {
    return ret + 24 * 3600;
  } else {
    return ret;
  }
}

qreal IgcInfo::absoluteMinTime() {
  int hrs = fixList->at(0).timestamp.hour();
  int mins = fixList->at(0).timestamp.minute();
  int secs = fixList->at(0).timestamp.second();

  int time = hrs*3600 + mins*60 + secs;
  return time;
}

qreal IgcInfo::absoluteTime(int i) {
  if ((i < 0) || (i >= count())) {
    return -1;
  }

  int hrs = fixList->at(i).timestamp.hour();
  int mins = fixList->at(i).timestamp.minute();
  int secs = fixList->at(i).timestamp.second();

  qreal time = hrs*3600 + mins*60 + secs;

  if (time < absoluteMinTime()) {
    return time + 24 * 3600;
  } else {
    return time;
  }
}

QTime IgcInfo::timestamp(int i) {
  QTime time;
  if ((i < 0) || (i > count())) return time;
  return fixList->at(i).timestamp;
}

qreal IgcInfo::absoluteMaxTime() {
  return absoluteTime(count()-1);
}

void IgcInfo::resetGlobalScale() {
  globalMin_ = min_;
  globalMax_ = max_;
  globalRobustMin_ = robustMin_;
  globalRobustMax_ = robustMax_;
}

void IgcInfo::addGlobalScale(const IgcInfo* other) {
  globalMin_ = qMin(other->globalMin_, globalMin_);
  globalMax_ = qMax(other->globalMax_, globalMax_);
  globalRobustMin_ = qMin(other->globalRobustMin_, globalRobustMin_);
  globalRobustMax_ = qMax(other->globalRobustMax_, globalRobustMax_);
}

int IgcInfo::indexOfTime(QTime time) {
  for (int i = 0; i < count(); i++) {
    if (fixList->at(i).timestamp == time) {
      return i;
    }
  }
  return -1;
}

qreal AltitudeIgcInfo::value(int i) const {
  return fixList->at(i).location.alt;
}

qreal SpeedIgcInfo::value(int i) const {
  if (fixList->count() < 2) {
    // This is a protection against malicious IGC files.
    return 0;
  }

  if (i == 0) {
    return speedBefore(1);
  } else if (i == fixList->count() - 1) {
    return speedBefore(i);
  } else {
    return (speedBefore(i + 1) + speedBefore(i)) / 2;
  }
}

qreal GroundSpeedIgcInfo::value(int i) const {
  return SpeedIgcInfo::value(i) * 3.6;
}

qreal SpeedIgcInfo::speedBefore(int i) const {
  int seconds = fixList->at(i - 1).timestamp.secsTo(fixList->at(i).timestamp);

  if (seconds <= 0) {
    // Since only time and not date is stored, it's possible that we cross
    // midnight and get negative value here. Improbable, though.
    seconds += 24 * 3600;
  }
    // The speed in m/s:
  return (this->distanceBefore(i) / seconds);
}

qreal GroundSpeedIgcInfo::distanceBefore(int i) const {
  const TrackFix &f1 = fixList->at(i - 1);
  const TrackFix &f2 = fixList->at(i);

  return qSqrt(
    (f2.x - f1.x) * (f2.x - f1.x) +
    (f2.y - f1.y) * (f2.y - f1.y) +
    (f2.z - f1.z) * (f2.z - f1.z));
}

qreal VerticalSpeedIgcInfo::distanceBefore(int i) const {
  return fixList->at(i).location.alt - fixList->at(i - 1).location.alt;
}

TrackIdIgcInfo::TrackIdIgcInfo(int id)
  : id(id) {}

qreal TrackIdIgcInfo::value(int i) const {
  return id;
}

void TimeIgcInfo::init(const QList<TrackFix> *fixList) {
  this->fixList = fixList;

  if (fixList->count() >= 1) {
    min_ = robustMin_ = 0;
    max_ = robustMax_ = value(fixList->count() - 1);
  }

  resetGlobalScale();
}

qreal TimeIgcInfo::value(int i) const {
  int ret = fixList->at(0).timestamp.secsTo(fixList->at(i).timestamp);

  if (ret < 0) {
    return ret + 24 * 3600;
  } else {
    return ret;
  }
}

void SegmentInfo::init(const QList<TrackFix>* fixList_) {
  fixList = fixList_;
}

qreal SegmentInfo::avgSpeed(int startPosition, int endPosition) {
  Util::Location startLocation = fixList->at(startPosition).location;
  Util::Location endLocation = fixList->at(endPosition).location;
  QTime start = fixList->at(startPosition).timestamp;
  QTime end = fixList->at(endPosition).timestamp;

  int startSecs = start.hour()*3600 + start.minute()*60 + start.second();
  int endSecs = end.hour()*3600 + end.minute()*60 + end.second();

  if (endSecs < startSecs) endSecs += 24*3600;
  qreal timeDiff = endSecs - startSecs;

  qreal distance = g_core->getEllipsoid()->
    distance(startLocation, endLocation);
  return (distance / timeDiff)*3.6;
}

qreal SegmentInfo::avgSpeed(QTime start, QTime end) {
  int spos;
  int epos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      spos = i;
    }
    if (fixList->at(i).timestamp == end) {
      epos = i;
    }
  }
  return avgSpeed(spos, epos);
}

qreal SegmentInfo::avgSpeedOverall() {
  return avgSpeed(0, fixList->count()-1);
}

qreal SegmentInfo::avgSpeedSince(QTime start) {
  int spos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      spos = i;
    }
  }
  return avgSpeed(spos, fixList->count()-1);
}

qreal SegmentInfo::avgSpeedUntil(QTime end) {
  int epos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == end) {
      epos = i;
    }
  }
  return avgSpeed(0, epos);
}

qreal SegmentInfo::avgRise(int startPosition, int endPosition) {
  qreal startHeight = fixList->at(startPosition).location.alt;
  qreal endHeight = fixList->at(endPosition).location.alt;
  QTime start = fixList->at(startPosition).timestamp;
  QTime end = fixList->at(endPosition).timestamp;

  int startSecs = start.hour()*3600 + start.minute()*60 + start.second();
  int endSecs = end.hour()*3600 + end.minute()*60 + end.second();

  if (endSecs < startSecs) endSecs += 24*3600;
  qreal timeDiff = endSecs - startSecs;

  return ((endHeight - startHeight) / timeDiff);
}

qreal SegmentInfo::avgRise(QTime start, QTime end) {
  int spos;
  int epos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      spos = i;
    }
    if (fixList->at(i).timestamp == end) {
      epos = i;
    }
  }
  return avgRise(spos, epos);
}

qreal SegmentInfo::avgRiseOverall() {
  return avgRise(0, fixList->count()-1);
}

qreal SegmentInfo::avgRiseSince(QTime start) {
  int spos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      spos = i;
    }
  }
  return avgRise(spos, fixList->count()-1);
}

qreal SegmentInfo::avgRiseUntil(QTime end) {
  int epos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == end) {
      epos = i;
    }
  }
  return avgRise(0, epos);
}

qreal SegmentInfo::distance(int startPos, int endPos) {
  Util::Location startLocation = fixList->at(startPos).location;
  Util::Location endLocation = fixList->at(endPos).location;

  qreal dist = g_core->getEllipsoid()->
    distance(startLocation, endLocation);

  return dist;
}

qreal SegmentInfo::distance(QTime start, QTime end) {
  int startPos;
  int endPos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      startPos = i;
    }
    if (fixList->at(i).timestamp == end) {
      endPos = i;
    }
  }
  return distance(startPos, endPos);
}

qreal SegmentInfo::distanceOverall() {
  return distance(0, fixList->count()-1);
}

qreal SegmentInfo::distanceSince(QTime start) {
  int startPos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      startPos = i;
    }
  }
  return distance(startPos, fixList->count()-1);
}

qreal SegmentInfo::distanceUntil(QTime end) {
  int endPos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == end) {
      endPos = i;
    }
  }
  return distance(0, endPos);
}


qreal SegmentInfo::heightDifference(int startPos, int endPos) {
  return fixList->at(endPos).location.alt - fixList->at(endPos).location.alt;
}

qreal SegmentInfo::heightDifference(QTime start, QTime end) {
  int startPos;
  int endPos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      startPos = i;
    }
    if (fixList->at(i).timestamp == end) {
      endPos = i;
    }
  }
  return heightDifference(startPos, endPos);
}

qreal SegmentInfo::heightDifferenceOverall() {
  return heightDifference(0, fixList->count()-1);
}

qreal SegmentInfo::heightDifferenceSince(QTime start) {
  int startPos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == start) {
      startPos = i;
    }
  }
  return heightDifference(startPos, fixList->count()-1);
}

qreal SegmentInfo::heightDifferenceUntil(QTime end) {
  int endPos;
  for (int i = 0; i < fixList->count(); i++) {
    if (fixList->at(i).timestamp == end) {
      endPos = i;
    }
  }
  return heightDifference(0, endPos);
}


QTime SegmentInfo::getStartTime() {
  return fixList->first().timestamp;
}

QTime SegmentInfo::getEndTime() {
  return fixList->last().timestamp;
}

}  // End namespace IgcViewer
}  // End namespace Updraft

