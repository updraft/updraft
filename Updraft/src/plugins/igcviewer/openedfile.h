#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

#include <QObject>
#include <QFileInfo>
#include <QList>
#include <QTextEdit>

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/AutoTransform>

#include "colorings.h"
#include "igcinfo.h"
#include "igcviewer.h"
#include "plotwidget.h"
#include "../../eventinfo.h"

class IGCViewer;

namespace Updraft {
namespace Igc {
  class IgcFile;
}
}

namespace Updraft {
namespace IgcViewer {

/// Helper class representing a single opened IGC file.
class OpenedFile: public QObject {
  Q_OBJECT

 public:
  /// Destructor. Also removes the file from the list
  /// of opened files in IgcViewer.
  ~OpenedFile();

  /// Open the file with the given filename.
  /// \param viewer The parent IgcViewer object
  /// \param filename The name of the file which should be opened
  /// \param color Color used for automatic coloring.
  /// \return Whether the file was successfully opened.
  bool init(IgcViewer* viewer, const QString& filename, QColor color);

  /// Force redraw of everything.
  void redraw();

  /// Reset the global scales.
  void resetScales();

  /// Update the global scales based on the other opened file.
  void updateScales(const OpenedFile* other);

  QColor getAutomaticColor() {
    return automaticColor;
  }

  void selectTab();

  /// Return the absolute file name.
  QString fileName();

  /// Set colors of the track according to the value selected in the viewer.
  void coloringChanged();

  osg::Node* getNode();

  /// If the track is clicked, it computes the nearest fix to the click point,
  /// and places a marker on that fix.
  /// Afterwards, it emits a signal with the fix-index.
  void trackClicked(const EventInfo* eventInfo);

 public slots:
  /// A slot waiting for the signal from the PlotWidget.
  /// It creates a new the marker for the picked fix,
  /// and places it at the position of the fix.
  void fixPicked(int index);

  /// A slot waiting for the signal from the PlotWidget.
  /// It sets the position of the marker of the current fix
  /// the user is pointing at with mouse.
  void fixIsPointedAt(int index);

  /// It clears the markers on the track.
  void clearMarkers();

  /// One of the map layers has requested a context menu.
  void contextMenuRequested(QPoint pos, MapLayerInterface* sender);

 private slots:
  /// Slot that gets called when the tab associated with this file is closed.
  /// Deletes the opened file.
  void close();

 private:
  /// Load the igc file to our own representation and close it.
  /// Fills fixList.
  bool loadIgc(const QString& filename);

  /// Create whole track in map.
  void createGroup();

  /// Create the track geometry in map.
  osg::Node* createTrack();

  /// Create the skirt under the track.
  osg::Node* createSkirt();

  /// Set coloring of the track.
  void setColors(Coloring* coloring);

  /// Create the marker geometry.
  osg::Geode* createMarker(qreal scale);

  /// Fill the header label information - name of the pilot, date, etc.
  void setHeaderText(QLabel* label);

  QFileInfo fileInfo;

  QComboBox *colorsCombo;
  IgcTextWidget* textBox;
  PlotWidget* plotWidget;

  TabInterface *tab;
  MapLayerInterface* track;
  IgcViewer *viewer;

  QColor automaticColor;

  /// Geometry of the 3D track visualisation.
  /// Used for coloring.
  osg::Geometry* geom;
  osg::Group* sceneRoot;
  osg::Geode* trackGeode;

  /// The geometry of the track marker.
  osg::ref_ptr<osg::Geode> trackPositionMarker;

  /// The transformations plus the geometry of the
  /// markers on picked fix positions.
  QList<osg::AutoTransform*> pickedMarkers;

  /// The transformation plus the marker of the fix
  /// the user is currently pointing at in the graph widget.
  /// Mouse pointing on the track is not implemented.
  osg::AutoTransform* currentMarker;

  Coloring* currentColoring;

  /// Igc file
  Igc::IgcFile* igc;

  /// List of track points.
  QList<TrackFix> fixList;

  QList<Coloring*> colorings;

  /// This variable contains all available igc infos accessible for mass
  /// rescaling / deleting / whatever.
  /// There are separate variables for named access to specific info classes.
  QList<FixInfo*> fixInfo;

  FixInfo* altitudeInfo;
  FixInfo* verticalSpeedInfo;
  FixInfo* groundSpeedInfo;
  FixInfo* timeInfo;

  Util::Gradient gradient;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

