#include "igcinfo.h"

namespace Updraft {
namespace IgcViewer {

void IgcInfo::init(const QList<TrackFix> *fixList) {
  this->fixList = fixList;

  min_ = this->value(0);
  max_ = min_;

  for (int i = 1; i < fixList->count(); ++i) {
    qreal v = value(i);

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

QColor IgcInfo::color(int i) {
  qreal scaled = (value(i) - globalMin_) / (globalMax_ - globalMin_);
  return colorFromScaled(scaled);
}

AltitudeIgcInfo::AltitudeIgcInfo()
  : g(Qt::darkGray, Qt::red) {}

qreal AltitudeIgcInfo::value(int i) {
  return fixList->at(i).location.alt;
}

QColor AltitudeIgcInfo::colorFromScaled(qreal scaled) {
  return g.get(scaled);
}

GroundSpeedIgcInfo::GroundSpeedIgcInfo()
  : g(Qt::darkGray, Qt::blue) {}

qreal GroundSpeedIgcInfo::value(int i) {
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

qreal GroundSpeedIgcInfo::speedBefore(int i) {
  const TrackFix &f1 = fixList->at(i - 1);
  const TrackFix &f2 = fixList->at(i);

  qreal dist = qSqrt(
    (f2.x - f1.x) * (f2.x - f1.x) +
    (f2.y - f1.y) * (f2.y - f1.y) +
    (f2.z - f1.z) * (f2.z - f1.z));

  int seconds = f1.timestamp.secsTo(f2.timestamp);

  if (seconds <= 0) {
    // Since only time and not date is stored, it's possible that we cross
    // midnight and get negative value here. Improbable, though.
    seconds += 24 * 3600;
  }

  return dist / seconds;
}

QColor GroundSpeedIgcInfo::colorFromScaled(qreal scaled) {
  return g.get(scaled);
}

void VerticalSpeedIgcInfo::init(const QList<TrackFix> *fixList) {
  this->fixList = fixList;

  max_ = this->value(0);

  for (int i = 1; i < fixList->count(); ++i) {
    qreal v = value(i);

    if (v < -max_) {
      max_ = -v;
    } else if (v > max_) {
      max_ = v;
    }
  }

  min_ = -max_;

  resetGlobalScale();
}

VerticalSpeedIgcInfo::VerticalSpeedIgcInfo()
  : positiveGradient(Qt::darkGray, Qt::green),
  negativeGradient(Qt::darkGray, Qt::red) {}

qreal VerticalSpeedIgcInfo::value(int i) {
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

qreal VerticalSpeedIgcInfo::speedBefore(int i) {
  const TrackFix &f1 = fixList->at(i - 1);
  const TrackFix &f2 = fixList->at(i);

  qreal dist = f2.location.alt - f1.location.alt;
  int seconds = f1.timestamp.secsTo(f2.timestamp);

  if (seconds <= 0) {
    // Since only time and not date is stored, it's possible that we cross
    // midnight and get negative value here. Improbable, though.
    seconds += 24 * 3600;
  }

  return dist / seconds;
}

QColor VerticalSpeedIgcInfo::colorFromScaled(qreal scaled) {
  if (scaled > 0.5) {
    return positiveGradient.get(scaled * 2 - 1);
  } else {
    return negativeGradient.get(scaled * 2);
  }
}

}  // End namespace IgcViewer
}  // End namespace Updraft

