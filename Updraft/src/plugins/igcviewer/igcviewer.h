#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCVIEWER_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCVIEWER_H_

#include <QtCore>
#include "../../pluginbase.h"

namespace Updraft {
namespace IgcViewer {

class OpenedFile;

class Q_DECL_EXPORT IgcViewer: public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();

  bool fileOpen(const QString &filename, int roleId);
  void fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename);

 private:
  /// Remove the opened file from the lists and notify recalculate
  /// all scales.
  void fileClose(OpenedFile* f);

  QList<OpenedFile*> opened;
  MapLayerGroupInterface* mapLayerGroup;

  int openedCounter;

  friend class OpenedFile;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCVIEWER_H_

