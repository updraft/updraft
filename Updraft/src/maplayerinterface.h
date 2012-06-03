#ifndef UPDRAFT_SRC_MAPLAYERINTERFACE_H_
#define UPDRAFT_SRC_MAPLAYERINTERFACE_H_

class QObject;
class QTreeWidgetItem;

namespace Updraft {

namespace Core {
  class MapLayerGroup;
  class MainWindow;
}

/// An interface to a map layer that is used by plugins.
/// More about map layers \see class MapLayer.
class MapLayerInterface {
 public:
  virtual ~MapLayerInterface() {}

  /// Connects to a signal checked(MapLayerInterface*, bool):
  /// map layer is selected / diselected in the left panel.
  /// \see QObject::connect()
  virtual void connectSignalChecked(const QObject* receiver,
    const char *method) = 0;

  /// Connects to a slot setVisibility(bool) -- map layer sets
  /// the visibility to the layer in the scene graph.
  /// \see QObject::connect()
  virtual void connectSlotSetVisibility(const QObject* sender,
    const char *method) = 0;

  /// Automatically connects the signal whether the map is
  /// selected / deselected in the menu to the
  /// setVisibility slot, so that the map enables / disabled it's
  /// visibility.
  virtual void connectCheckedToVisibility() = 0;

  /// Change the title of this map layer.
  virtual void setTitle(const QString& title) = 0;

  /// Check or uncheck the tree view checkbox
  virtual void setChecked(bool value) = 0;

  /// Allows to disable the check box.
  virtual void setCheckable(bool value) = 0;

  /// Sets visibility to the layer.
  virtual void setVisibility(bool value) = 0;

  virtual bool isVisible() = 0;

  friend class Core::MapLayerGroup;
  friend class Core::MainWindow;

 protected:
  virtual void emitChecked(bool value) = 0;

  /// Add the map layer to the osg group of the map layer group.
  virtual void addToScene(Core::MapLayerGroup *group) = 0;

  /// Remove the layer from the osg group of the map layer group.
  virtual void removeFromScene(Core::MapLayerGroup *group) = 0;

  /// Return the tree widget item corresponding to this map layer.
  virtual QTreeWidgetItem* getTreeItem() = 0;

  /// Notify the map layer that it was inserted into a group.
  virtual void inserted(Core::MapLayerGroup* parent) = 0;
  virtual void setDisabled(bool disabled) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MAPLAYERINTERFACE_H_
