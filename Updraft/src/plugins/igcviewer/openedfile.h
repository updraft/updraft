#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

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
  ~OpenedFile();

  bool init(IgcViewer* viewer, const QString& filename);

 private slots:
  /// Slot that gets called when the tab associated with this file gets closed.
  /// Deletes the opened file.
  void tabClosed();

 private:
  /// Create a tab with the flight recording's graph and controls.
  void createTab();

  /// Create the track in map.
  void createTrack();

  QFileInfo fileInfo;

  TabInterface *tab;
  MapLayerInterface* track;
  IgcViewer *viewer;

  Igc::IgcFile igc;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

