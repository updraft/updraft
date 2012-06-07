#ifndef UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_
#define UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_

#include <QtGui>
#include <QAction>
#include "../../pluginbase.h"
#include "oaengine.h"
#include "../../maplayerinterface.h"

namespace Updraft {
namespace Airspaces {

/// Top level object of airspaces plugin.
class Q_DECL_EXPORT Airspaces: public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  /// The class constructor.
  Airspaces();

  /// \return The plug-in name.
  QString getName();

  /// Plug-in initialisation routine.
  /// \param coreInterface The pointer to the Core.
  void initialize(CoreInterface *coreInterface);

  /// Plug-in unlouad.
  void deinitialize();

  /// Open File routine.
  /// Processes the imported file according to
  /// the file type calls the correct drawing engine.
  /// \param fileName The filename.
  /// \param role The file type.
  bool fileOpen(const QString& fileName, int role);

  /// Loads allt he files imported to the application.
  void loadImportedFiles();

 public slots:
  /// Changes the visibility of the plug-in.
  void mapLayerDisplayed(bool value, MapLayerInterface* sender);

  /// Reloads the airspaces.
  void reloadAirspaces();

  /// Context menu setup.
  void contextMenuRequested(QPoint pos, MapLayerInterface* sender);

 private:
  enum FileRole {
    IMPORT_OPENAIRSPACE_FILE = 0
  };

  /// Registration for loading Airspaces from OpenAirspace file.
  FileRegistration OAirspaceFileReg;

  /// Map nodes.
  QVector<QPair<osg::Node*, QString> >* mapNodes;

  /// Map layers.
  MapLayerGroupInterface* mapLayerGroup;
};

}  // End namespace Airspaces
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_

