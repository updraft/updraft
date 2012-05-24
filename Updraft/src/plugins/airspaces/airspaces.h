#ifndef UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_
#define UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_

#include <QtGui>
#include "../../pluginbase.h"
#include "oaengine.h"
#include "../../maplayerinterface.h"

namespace Updraft {
namespace Airspaces {

class Q_DECL_EXPORT Airspaces: public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  Airspaces();

  QString getName();

  void initialize(CoreInterface *coreInterface);

  void deinitialize();

  bool fileOpen(const QString&, int);

  void loadImportedFiles();

 public slots:
  void mapLayerDisplayed(bool value, MapLayerInterface* sender);

  void reloadAirspaces();

 private:
  enum FileRole {
    IMPORT_OPENAIRSPACE_FILE = 0
  };
  /// Registration for loading Airspaces from OpenAirspace file.
  FileRegistration OAirspaceFileReg;

  QVector<MapLayerInterface*>* mapLayers;
  QVector<QPair<osg::Node*, QString> >* mapNodes;
  // oaEngine* engine;
  MapLayerGroupInterface* mapLayerGroup;
};

}  // End namespace Airspaces
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_

