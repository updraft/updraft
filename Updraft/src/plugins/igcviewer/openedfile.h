#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

#include <osg/Geode>

#include <QObject>
#include <QFileInfo>

#include "../../libraries/igc/igc.h"

#include "igcviewer.h"

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

 private:
  /// Create a tab with the flight recording's graph and controls.
  void createTab();

  /// Create the track in map.
  void createTrack();

  QFileInfo fileInfo;

  TabInterface *tab;
  MapLayerInterface* track;
  IgcViewer *viewer;

  osg::Geode* geode;

  Igc::IgcFile igc;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

