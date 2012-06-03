#ifndef UPDRAFT_SRC_CORE_MAPLAYER_H_
#define UPDRAFT_SRC_CORE_MAPLAYER_H_

#include <QObject>
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
  void connectSlotSetVisibility(const QObject* sender,
    const char *method);
  void connectCheckedToVisibility();

  /// Change the title of this map layer.
  void setTitle(const QString& title);

  QTreeWidgetItem *getTreeItem();
  void inserted(MapLayerGroup* parent);
  void setDisabled(bool disabled);
  void setChecked(bool value);

 public slots:
  void setVisibility(bool value) = 0;

 signals:
  /// Emited when the map layer is checked or unchecked
  /// (before it is displayed or hidden).
  void checked(bool value, MapLayerInterface* sender);

 protected:
  /// Emits the checked signal.
  /// Is called from MapLayerGroup.
  void emitChecked(bool value);

  explicit MapLayer(QTreeWidgetItem* item);

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
