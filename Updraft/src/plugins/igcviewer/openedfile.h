#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

#include <QComboBox>
#include <QObject>
#include <QFileInfo>
#include <QList>

#include <osg/Geometry>

#include "igc/igc.h"
#include "util/util.h"

#include "igcviewer.h"
#include "igcinfo.h"

namespace Updraft {
namespace IgcViewer {

/// Helper class representing a single opened IGC file.
class OpenedFile: public QObject {
  Q_OBJECT

 public:
  /// Don't use delete on this class.
  /// Use close() instead.
  ~OpenedFile();

  bool init(IgcViewer* viewer, const QString& filename);
  void redraw();

 public slots:
  /// Slot that gets called when the tab associated with this file gets closed.
  /// Deletes the opened file.
  /// This slot is the only way to destroy this class.
  void close();

  /// Set colors of the track according to the value selected in the combo.
  void updateColors(int row);

 private:
  /// Create a tab with the flight recording's graph and controls.
  /// Must be called after createTrack(), since it sets the colors of the track.
  void createTab();

  /// Create the track in map.
  void createTrack();

  /// Set coloring of the track.
  void setColors(IgcInfo *coloring);

  QFileInfo fileInfo;

  TabInterface *tab;
  MapLayerInterface* track;
  IgcViewer *viewer;

  Igc::IgcFile igc;

  QComboBox *colorsCombo;

  osg::Geometry* geom;

  QList<TrackFix> fixList;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

