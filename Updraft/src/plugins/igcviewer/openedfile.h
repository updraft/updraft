#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

#include <QObject>
#include <QFileInfo>
#include <QList>

#include <osg/Geometry>

#include "igcviewer.h"
#include "igcinfo.h"

namespace Updraft {
namespace IgcViewer {

/// Helper class representing a single opened IGC file.
class OpenedFile: public QObject {
  Q_OBJECT

 public:
  /// Destructor. Also removes the file from the list
  /// of opened files in IgcViewer.
  ~OpenedFile();

  bool init(IgcViewer* viewer, const QString& filename);

  /// Force redraw of everything.
  void redraw();

 private slots:
  /// Slot that gets called when the tab associated with this file is closed.
  /// Deletes the opened file.
  void close();

  /// Set colors of the track according to the value selected in the combo.
  void updateColors(int row);

 private:
  /// Load the igc file to our own representation and close it.
  /// Fills fixList.
  bool loadIgc(const QString& filename);

  /// Create the track in map.
  void createTrack();

  /// Set coloring of the track.
  void setColors(IgcInfo* coloring);

  QFileInfo fileInfo;

  TabInterface *tab;
  MapLayerInterface* track;
  IgcViewer *viewer;

  /// Geometry of the 3D track visualisation.
  /// Used for coloring.
  osg::Geometry* geom;

  IgcInfo *currentColoring;

  /// List of track points.
  QList<TrackFix> fixList;

  /// This variable contains all available igc infos accessible for mass
  /// rescaling / delete / whatever.
  /// There are separate variables for named access to specific info classes.
  QList<IgcInfo*> igcInfoList;

  IgcInfo* altitudeInfo;
  IgcInfo* verticalSpeedInfo;

  friend class IgcViewer;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

