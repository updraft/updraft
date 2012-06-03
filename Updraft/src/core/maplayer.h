#ifndef UPDRAFT_SRC_CORE_MAPLAYER_H_
#define UPDRAFT_SRC_CORE_MAPLAYER_H_

#include <QObject>
#include <QPoint>
#include "../maplayerinterface.h"

class QTreeWidgetItem;

namespace Updraft {
namespace Core {

class MapLayerGroup;

/// A class that represents one logical layer in map.
/// Map layer in osgEarth can be of different type, and this
/// class encapsulates all of the types, and creates common
/// interface.
/// The type of the map layer determines which value of the
/// union Layer is valid.
class MapLayer : public QObject, virtual public MapLayerInterface {
  Q_OBJECT

 public:
  explicit MapLayer(const QString &title);
  ~MapLayer();

  void connectSignalChecked(const QObject* receiver,
    const char *method);
  void connectSignalContextMenuRequested(const QObject* receiver,
    const char* method);
  void connectSlotSetVisibility(const QObject* sender,
    const char *method);
  void connectCheckedToVisibility();

  /// Change the title of this map layer.
  void setTitle(const QString& title);

  QTreeWidgetItem *getTreeItem();
  void inserted(MapLayerGroup* parent);
  void setDisabled(bool disabled);
  void setChecked(bool value);
  void setCheckable(bool value);

  void setFilePath(const QString& path);
  QAction* getDeleteAction();

 public slots:
  void setVisibility(bool value) = 0;

 private slots:
  void deleteActionSlot();

 signals:
  /// Emited when the map layer is checked or unchecked
  /// (before it is displayed or hidden).
  void checked(bool value, MapLayerInterface* sender);
  void contextMenuRequested(QPoint pos, MapLayerInterface* sender);

 protected:
  /// Emits the checked signal.
  /// Called from MainWindow.
  void emitChecked(bool value);
  void emitContextMenuRequested(const QPoint& pos);

  explicit MapLayer(QTreeWidgetItem* item);

  /// Action for getDeleteAction.
  QAction* deleteAction;

  /// File path for delete action.
  QString deleteFilePath;

  /// Tree item representing this map layer.
  QTreeWidgetItem *treeItem;

  /// If this is true, then the treeItem is deleted together
  /// with the mapLayer.
  /// This is only false for the top level group.
  bool ownsTreeItem;

  /// Parent map layer group.
  /// Used when using operator delete on the layer.
  /// This is set by the parent group.
  MapLayerGroup* parent;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPLAYER_H_
