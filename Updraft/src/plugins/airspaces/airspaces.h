#ifndef UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_
#define UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_

#include <QtGui>
#include "../../pluginbase.h"
#include "oaengine.h"

namespace Updraft {
namespace Airspaces {

class Q_DECL_EXPORT Airspaces: public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  Airspaces();

  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();

  bool fileOpen(const QString&, int);

  void loadImportedFiles();

 public slots:
  void mapLayerDisplayed(bool value, MapLayerInterface* sender);

 private:
  enum FileRole {
    IMPORT_OPENAIRSPACE_FILE = 0
  };
  /// Registration for loading Airspaces from OpenAirspace file.
  FileRegistration OAirspaceFileReg;

  QVector<MapLayerInterface*>* mapLayers;
  oaEngine* engine;
};

}  // End namespace Airspaces
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRSPACES_AIRSPACES_H_

