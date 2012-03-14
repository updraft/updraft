#include "igcinfo.h"

namespace Updraft {
namespace IgcViewer {

void IgcInfo::init(const QList<TrackFix> *fixList) {
  this->fixList = fixList;

  if (fixList->count() < 1) {
    min_ = max_ = 0;
    resetGlobalScale();
    return;
  }

  min_ = this->value(0);
  max_ = min_;

  for (int i = 1; i < fixList->count(); ++i) {
    qreal v = this->value(i);

    if (min_ > v) {
      min_ = v;
    }

    if (max_ < v) {
      max_ = v;
    }
  }

  resetGlobalScale();
}

void IgcInfo::resetGlobalScale() {
  globalMin_ = min_;
  globalMax_ = max_;
}

void IgcInfo::addGlobalScale(qreal minimum, qreal maximum) {
  globalMin_ = qMin(minimum, globalMin_);
  globalMax_ = qMax(maximum, globalMax_);
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

qreal SpeedIgcInfo::speedBefore(int i) const {
  int seconds = fixList->at(i - 1).timestamp.secsTo(fixList->at(i).timestamp);

  if (seconds <= 0) {
    // Since only time and not date is stored, it's possible that we cross
    // midnight and get negative value here. Improbable, though.
    seconds += 24 * 3600;
  }

  return this->distanceBefore(i) / seconds;
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
    min_ = 0;
    max_ = value(fixList->count() - 1);
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

}  // End namespace IgcViewer
}  // End namespace Updraft

