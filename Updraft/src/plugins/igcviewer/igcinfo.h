#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCINFO_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCINFO_H_

#include <QTime>
#include <QList>

#include "util/util.h"

namespace Updraft {
namespace IgcViewer {

/// Structure representing a single point of the igc recording,
/// already projected and prepared for displaying.
struct TrackFix {
  TrackFix(QTime timestamp, Util::Location location,
    double x, double y, double z, qreal terrainHeight) :
    timestamp(timestamp), location(location), x(x), y(y), z(z),
    terrainHeight(terrainHeight) {}

  QTime timestamp;

  /// Location of the fix
  Util::Location location;

  /// Projected location of the fix
  qreal x, y, z;

  /// Terrain height at the position of the fix.
  qreal terrainHeight;
};

/// Base for track computing values and scales from igc recording.
/// For multiple opened tracks the colorings can scale together,
/// this is called global scale in the code.
class IgcInfo {
 public:
  virtual ~IgcInfo() {}

  /// Initialize the coloring.
  /// \param fixList List of fixes, usable for scaling, smoothing, ...
  virtual void init(const QList<TrackFix> *fixList);

  /// Get the raw value of item number i.
  /// \pre i >= 0 && i < this->count()
  virtual qreal value(int i) const = 0;

  /// Return the index of the item with the given timestamp.
  virtual int indexOfTime(QTime time);

  //// Get the relative time in seconds of item i.
  //// \pre i >= 0 && i < this->count()
  qreal time(int i) const;

  /// Return relative time in seconds of the last item.
  qreal maxTime() const { return time(count() - 1); }

  //// Get the absolute time in seconds of item i.
  //// \pre i >= 0 && i < this->count()
  qreal absoluteTime(int i);

  /// Get absolute timestamp of item i.
  QTime timestamp(int i);

  /// Return absolute time in seconds of the last item.
  qreal absoluteMaxTime();

  /// Return absolute time in seconds of the first item.
  qreal absoluteMinTime();

  /// Forget any values previously associated with the global scale.
  virtual void resetGlobalScale();

  /// Propagate the global scales from the other igc info.
  virtual void addGlobalScale(const IgcInfo* other);

  /// Number of items in the underlying fix list.
  int count() const { return fixList->count(); }

  /// Return the minimal value of this track.
  qreal min() const { return min_; }

  /// Return the maximal value of this track.
  qreal max() const { return max_; }

  /// Return the outlier-free minimal value of this track.
  /// \note This is generally inside the (min, max) interval.
  qreal robustMin() const { return robustMin_; }

  /// Return the outlier-free maximal value of this track.
  /// \note This is generally inside the (min, max) interval.
  qreal robustMax() const { return robustMax_; }

  /// Return the minimal value of all tracks.
  qreal globalMin() const { return globalMin_; }

  /// Return the maximal value of all tracks.
  qreal globalMax() const { return globalMax_; }

  /// Return the outlier-free minimal value of all tracks.
  /// \note This is generally inside the (globalMin, globalMax) interval.
  qreal globalRobustMin() const { return globalRobustMin_; }

  /// Return the outlier-free maximal value of all tracks.
  /// \note This is generally inside the (globalMin, globalMax) interval.
  qreal globalRobustMax() const { return globalRobustMax_; }

 protected:
  const QList<TrackFix> *fixList;
  qreal min_, max_;
  qreal robustMin_, robustMax_;
  qreal globalMin_, globalMax_;
  qreal globalRobustMin_, globalRobustMax_;
};

/// Returns altitude.
class AltitudeIgcInfo : public IgcInfo {
 public:
  qreal value(int i) const;
};

/// Abstract helper for simplifying info classes with speeds.
class SpeedIgcInfo : public IgcInfo {
 public:
  qreal value(int i) const;

 private:
  /// Return the speed of the segment before i
  /// (corresponding to distanceBefore(i)).
  /// The value is in m/s.
  qreal speedBefore(int i) const;

  /// Return the distance of the segment before i (between fixes i-1 and i)
  virtual qreal distanceBefore(int i) const = 0;
};

/// Calculates GPS speed.
class GroundSpeedIgcInfo : public SpeedIgcInfo {
 public:
  qreal value(int i) const;

 private:
  qreal distanceBefore(int i) const;
};

/// Caclulate vertical speed.
class VerticalSpeedIgcInfo : public SpeedIgcInfo {
 private:
  qreal distanceBefore(int i) const;
};

class TrackIdIgcInfo : public IgcInfo {
 public:
  explicit TrackIdIgcInfo(int id);

  qreal value(int i) const;
 private:
  int id;
};

/// Returns altitude.
class TimeIgcInfo : public IgcInfo {
 public:
  void init(const QList<TrackFix> *fixList);
  qreal value(int i) const;
 private:
};

class SegmentInfo {
 public:
  void init(const QList<TrackFix>* fixList_);
  qreal avgSpeed(QTime start, QTime end);
  qreal avgSpeedOverall();
  qreal avgSpeedSince(QTime start);
  qreal avgSpeedUntil(QTime end);
  qreal avgRise(QTime start, QTime end);
  qreal avgRiseOverall();
  qreal avgRiseSince(QTime start);
  qreal avgRiseUntil(QTime end);
  qreal distance(QTime start, QTime end);
  qreal distanceOverall();
  qreal distanceSince(QTime start);
  qreal distanceUntil(QTime end);
  qreal heightDifference(QTime start, QTime end);
  qreal heightDifferenceOverall();
  qreal heightDifferenceSince(QTime start);
  qreal heightDifferenceUntil(QTime end);

  QTime getStartTime();
  QTime getEndTime();
 private:
  const QList<TrackFix>* fixList;
  qreal avgSpeed(int startPosition, int endPosition);
  qreal avgRise(int startPosition, int endPosition);
  qreal distance(int startPosition, int endPosition);
  qreal heightDifference(int startPosition, int endPosition);
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCINFO_H_

