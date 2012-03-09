#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_COLORINGS_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_COLORINGS_H_

#include <QColor>
#include <QList>

#include "openedfile.h"

namespace Updraft {
namespace IgcViewer {

/// Base for track computing recording colors.
class Coloring {
 public:
  virtual ~Coloring() {}

  /// Initialize the coloring.
  /// \param fixList List of fixes, usable for scaling, smoothing, ...
  virtual void init(const QList<TrackFix> *fixList);

  /// Return the color for vertex.
  /// \param i Index of the fix.
  virtual QColor color(int i);

 protected:
  //// Find the value of item number i, without scaling.
  virtual qreal value(int i) = 0;

  /// Convert scaled value to color.
  virtual QColor colorFromScaled(qreal scaled) = 0;

  const QList<TrackFix> *fixList;
  qreal min;
  qreal max;
};

/// Color the track with constant color.
class ConstantColoring : public Coloring {
 public:
  explicit ConstantColoring(QColor val) : val(val) {}

  /// Do nothing.
  void init(const QList<TrackFix> *fixList) {}

  /// Return the constant color
  QColor color(int i) { return val; }

 protected:
  /// Do nothing.
  virtual qreal value(int i) { return 0; }

  /// Do nothing.
  virtual QColor colorFromScaled(qreal scaled) { return QColor(); }

  QColor val;
};

/// Color the track according to altitude.
class AltitudeColoring : public Coloring {
 protected:
  //// Find the value of item number i, without scaling.
  virtual qreal value(int i);

  /// Convert scaled value to color.
  virtual QColor colorFromScaled(qreal scaled);
};

/// Color the track according to GPS speed.
class GroundSpeedColoring : public Coloring {
 protected:
  //// Find the value of item number i, without scaling.
  virtual qreal value(int i);

  /// Return the speed of the segment before i (fixes no i and i-1)
  qreal speedBefore(int i);

  /// Convert scaled value to color.
  /// \param scaled Scaled value from 0 to 255.
  virtual QColor colorFromScaled(qreal scaled);
};

}  // End namespace IgcViewer
}  // End namespace Updraft

Q_DECLARE_METATYPE(Updraft::IgcViewer::Coloring*)

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_COLORINGS_H_

