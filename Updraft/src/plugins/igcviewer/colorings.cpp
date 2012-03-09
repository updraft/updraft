#include "colorings.h"

namespace Updraft {
namespace IgcViewer {

void Coloring::init(const QList<TrackFix> *fixList) {
  this->fixList = fixList;

  min = this->value(0);
  max = min;

  for (int i = 1; i < fixList->count(); ++i) {
    qreal v = value(i);

    // We want negative values to scale symmetrically.
    if (v < 0) {
      min = 0;
      v = -v;
    }

    if (min > v) {
      min = v;
    }

    if (max < v) {
      max = v;
    }
  }
}

QColor Coloring::color(int i) {
  qreal scaled = (value(i) - min) / (max - min);
  return colorFromScaled(scaled);
}

AltitudeColoring::AltitudeColoring()
  : g(Qt::darkGray, Qt::red) {}

qreal AltitudeColoring::value(int i) {
  return fixList->at(i).location.alt;
}

QColor AltitudeColoring::colorFromScaled(qreal scaled) {
  return g.get(scaled);
}

GroundSpeedColoring::GroundSpeedColoring()
  : g(Qt::darkGray, Qt::blue) {}

qreal GroundSpeedColoring::value(int i) {
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

qreal GroundSpeedColoring::speedBefore(int i) {
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

QColor GroundSpeedColoring::colorFromScaled(qreal scaled) {
  return g.get(scaled);
}

}  // End namespace IgcViewer
}  // End namespace Updraft

