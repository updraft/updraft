#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_

#include <QtGui>
#include "../../pluginbase.h"
#include "../../core/ui/maplayergroup.h"

#include "tplayer.h"

#ifdef turnpoints_EXPORTS
# define TPS_EXPORT Q_DECL_EXPORT
#else
# define TPS_EXPORT Q_DECL_IMPORT
#endif

namespace Updraft {

typedef QMap<MapLayerInterface*, TPLayer*> TTPLayerMap;

class TPS_EXPORT TurnPoints : public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  TurnPoints();

  virtual ~TurnPoints();

  QString getName();

  unsigned getPriority();

  void initialize(CoreInterface *coreInterface);

  void deinitialize();

  bool fileOpen(const QString &filename, int roleId);

  void fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename);

  void fillContextMenu(MapObject* obj, MenuInterface* menu);
  bool wantsToHandleClick(MapObject* obj);
  void handleClick(MapObject* obj, const EventInfo* evt);
 public slots:
  void mapLayerDisplayed(bool value, MapLayerInterface* sender);

 private:
  enum FileRoles {
    IMPORT_CUP_FILE = 1
  };

  /// List of imported files
  TTPLayerMap layers;

  /// Turn-points map layer group
  MapLayerGroupInterface *mapLayerGroup;

  /// Registration for loading turn-points from cup file.
  FileRegistration cupTPsReg;

  /// Loads all files in import directories used by this plugin.
  void loadImportedFiles();

  /// Removes instances of all loaded files.
  void unloadFiles();

  /// Creates new layer item, adds item to the left pane
  /// \param file associated data file (is deleted on layer destruction)
  void addLayer(TPFile *file);

  /// TP settings to be stored in settings array.
  QVector<SettingInterface*> settings;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_
