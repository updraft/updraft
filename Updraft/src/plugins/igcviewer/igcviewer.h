#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCVIEWER_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCVIEWER_H_

#include <QColor>
#include <QList>
#include <QPair>
#include "../../pluginbase.h"
#include "../../mapobject.h"

namespace Updraft {
namespace IgcViewer {

class OpenedFile;

class IGCMapObject: public QObject, public MapObject {
  Q_OBJECT
 private:
  OpenedFile* file;
 public:
  IGCMapObject(QString objectName_, OpenedFile* file_);
  IGCMapObject() {}

  QObject* asQObject();
};

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

  /// Tells whether this plugin wants to handle a mouse click event.
  bool wantsToHandleClick(MapObject* obj);

  /// Handles the left mouse click event on the IGC in the map.
  void handleClick(MapObject* obj, const EventInfo* evt);

 private slots:
  /// One of the opened files changed its coloring.
  /// propagate this change to all of them.
  void coloringChanged(int i);

 private:
  /// Remove the opened file from the lists and notify recalculate
  /// all scales.
  void fileClose(OpenedFile* f);

  /// Finds a least used automatic color and increments its use count.
  QColor findAutomaticColor();

  /// Decrement use count for the given automatic color.
  void freeAutomaticColor(QColor c);

  /// Currently selected coloring.
  int currentColoring;

  QList<QPair<QColor, int> > automaticColors;

  QMap<QString, OpenedFile*> opened;
  MapLayerGroupInterface* mapLayerGroup;
  QVector<MapObject*> mapObjects;

  friend class OpenedFile;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_IGCVIEWER_H_

