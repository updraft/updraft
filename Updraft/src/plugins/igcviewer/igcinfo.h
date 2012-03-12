#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCINFO_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCINFO_H_

#include <QColor>
#include <QTime>
#include <QList>
#include <QMetaType>

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

/// Base for track computing values and colors from igc recording.
/// For multiple opened tracks the colorings can scale together,
/// this is called global scale in the code.
class IgcInfo {
 public:
  virtual ~IgcInfo() {}

  /// Initialize the coloring.
  /// \param fixList List of fixes, usable for scaling, smoothing, ...
  virtual void init(const QList<TrackFix> *fixList);

  /// Return the color for vertex.
  /// \param i Index of the fix.
  virtual QColor color(int i);

  //// Get the raw value of item number i.
  virtual qreal value(int i) = 0;

  /// Forget any values previously associated with the global scale.
  virtual void resetGlobalScale();

  /// Add a new minimum and maximum to the global scale;
  virtual void addGlobalScale(qreal minimum, qreal maximum);

  /// Return the minimal value of this track.
  qreal min() { return min_; }

  /// Return the maximal value of this track.
  qreal max() { return max_; }

  /// Return the minimal value of all tracks.
  qreal globalMin() { return globalMin_; }

  /// Return the maximal value of all tracks.
  qreal globalMax() { return globalMax_; }

 protected:
  /// Convert scaled value to color.
  /// \param scaled value from 0 to 1.
  virtual QColor colorFromScaled(qreal scaled) = 0;

  const QList<TrackFix> *fixList;
  qreal min_, max_;
  qreal globalMin_, globalMax_;
};

/// Color the track with constant color.
class ConstantIgcInfo : public IgcInfo {
 public:
  explicit ConstantIgcInfo(QColor val) : val(val) {}

  /// Do nothing.
  void init(const QList<TrackFix> *fixList) {}

  /// Return the constant color
  QColor color(int i) { return val; }

  /// Do nothing.
  virtual qreal value(int i) { return 0; }

 protected:
  /// Do nothing.
  virtual QColor colorFromScaled(qreal scaled) { return QColor(); }

  QColor val;
};

/// Color the track according to altitude.
class AltitudeIgcInfo : public IgcInfo {
 public:
  AltitudeIgcInfo();

  //// Find the value of item number i, without scaling.
  virtual qreal value(int i);

 protected:
  /// Convert scaled value to color.
  virtual QColor colorFromScaled(qreal scaled);

  Util::Gradient g;
};

/// Color the track according to GPS speed.
class GroundSpeedIgcInfo : public IgcInfo {
 public:
  GroundSpeedIgcInfo();

  //// Find the value of item number i, without scaling.
  virtual qreal value(int i);

 protected:
  /// Return the speed of the segment before i (fixes no i and i-1)
  qreal speedBefore(int i);

  /// Convert scaled value to color.
  /// \param scaled Scaled value from 0 to 255.
  virtual QColor colorFromScaled(qreal scaled);

  Util::Gradient g;
};

/// Color the track according to vertical speed.
class VerticalSpeedIgcInfo : public IgcInfo {
 public:
  VerticalSpeedIgcInfo();

  /// Calculate the scale symetrically to zero.
  void init(const QList<TrackFix> *fixList);

  //// Find the value of item number i, without scaling.
  virtual qreal value(int i);

 protected:
  /// Return the speed of the segment before i (fixes no i and i-1)
  qreal speedBefore(int i);

  /// Convert scaled value to color.
  /// \param scaled Scaled value from 0 to 255.
  virtual QColor colorFromScaled(qreal scaled);

  Util::Gradient positiveGradient;
  Util::Gradient negativeGradient;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

Q_DECLARE_METATYPE(Updraft::IgcViewer::IgcInfo*)

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCINFO_H_

