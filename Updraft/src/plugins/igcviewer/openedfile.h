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
namespace IgcViewer {

/// Helper class representing a single opened IGC file.
class OpenedFile: public QObject {
  Q_OBJECT

 public:
  /// Destructor. Also removes the file from the list
  /// of opened files in IgcViewer.
  ~OpenedFile();

  /// Open the filename.
  /// \param color Color used for automatic coloring.
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

  void trackClicked(const EventInfo* eventInfo);

 public slots:
  void timePicked(QTime time);
  void clearMarkers();

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

  osg::Geode* createMarker(qreal scale);

  QFileInfo fileInfo;

  QComboBox *colorsCombo;
  IGCTextWidget* textBox;
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
  osg::ref_ptr<osg::Geode> trackPositionMarker;
  QList<osg::AutoTransform*> currentMarkers;

  Coloring* currentColoring;

  /// List of track points.
  QList<TrackFix> fixList;

  QList<Coloring*> colorings;

  /// This variable contains all available igc infos accessible for mass
  /// rescaling / deleting / whatever.
  /// There are separate variables for named access to specific info classes.
  QList<IgcInfo*> igcInfo;

  IgcInfo* altitudeInfo;
  IgcInfo* verticalSpeedInfo;
  IgcInfo* groundSpeedInfo;
  IgcInfo* timeInfo;

  SegmentInfo* segmentInfo;

  Util::Gradient gradient;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_OPENEDFILE_H_

