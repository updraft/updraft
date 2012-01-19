#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_

#include <QtGui>
#include "../../pluginbase.h"
#include "../../core/ui/maplayergroup.h"

#include "tplayer.h"

namespace Updraft {

typedef QMap<int, TPLayer*> TTPLayerMap;

class Q_DECL_EXPORT TurnPoints : public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  TurnPoints();

  virtual ~TurnPoints();

  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();

  bool fileOpen(const QString &filename, int roleId);

  void fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename);

 private:
  enum FileRoles {
    IMPORT_CUP_FILE = 1
  };

  /// Value which will be used as a next layer id.
  int nextLayerId;

  /// List of imported files
  TTPLayerMap layers;

  /// Turn-points map layer group
  MapLayerGroup *mapLayerGroup;

  /// Registration for loading turn-points from cup file.
  FileRegistration cupTPsReg;

  /// Loads all files in import directories used by this plugin.
  void loadImportedFiles();

  /// Removes instances of all loaded files.
  void unloadFiles();

  /// Creates new layer item, adds item to the left pane
  /// \param file associated data file (is deleted on layer destruction)
  void addLayer(TPFile *file);
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_
