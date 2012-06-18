#ifndef UPDRAFT_SRC_MAPLAYERINTERFACE_H_
#define UPDRAFT_SRC_MAPLAYERINTERFACE_H_

class QObject;
class QTreeWidgetItem;
class QPoint;
class QAction;
class QString;

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

  /// Connects to a signal checked(bool, MapLayerInterface*):
  /// map layer is selected / diselected in the left panel.
  /// \see QObject::connect()
  virtual void connectSignalChecked(const QObject* receiver,
    const char *method) = 0;

  /// Connects to a signal contextMenuRequested(QPoint, MapLayerInterface*)
  /// \see QObject::connect()
  virtual void connectSignalContextMenuRequested(const QObject* receiver,
    const char* method) = 0;

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

  /// Set the identifier of this map layer.
  /// Identifiers default to the map title encoded to local 8 bit
  /// encoding, however when the title may change
  /// (during runtime or between start ups) or when the id contains characters
  /// that are not representable in local encoding, a fixed id must be added
  /// manually.
  /// Ids only need to be unique between direct descendants of a group.
  virtual void setId(const QByteArray& id) = 0;

  /// Change the title of this map layer.
  /// If you use setTitle, then you should also be using setId().
  virtual void setTitle(const QString& title) = 0;

  /// Check or uncheck the tree view checkbox
  virtual void setChecked(bool value) = 0;

  /// Allows to disable the check box.
  virtual void setCheckable(bool value) = 0;

  /// Sets visibility to the layer.
  virtual void setVisibility(bool value) = 0;

  /// Deleting imported files.
  /// \{

  /// Sets the path used by getDeleteAction.
  virtual void setFilePath(const QString& path) = 0;

  /// Return a QAction that will delete the file
  /// selected by setFilePath.
  /// Used for deleting imported files.
  /// Only makes sense after setFilePath was called.
  /// The action's triggered() signal deletes the selected file and
  /// this map layer group.
  /// For a single map layer, the same QAction is returned every time.
  virtual QAction* getDeleteAction() = 0;

  /// Returns pointer to the action that zooms to make everything associated
  /// with this map layer visible in the map.
  /// The action's triggered() signal zooms the current view.
  /// For a single map layer, the same QAction is returned every time.
  virtual QAction* getZoomAction() = 0;

  /// \}

  virtual bool isVisible() = 0;

  /// Zoom to make everything associated with this map layer visible in the map.
  /// Called when the zoomAction is triggered.
  /// \see getZoomAction()
  virtual void zoom() = 0;

  friend class Core::MapLayerGroup;
  friend class Core::MainWindow;

 protected:
  virtual void emitChecked(bool value) = 0;
  virtual void emitContextMenuRequested(const QPoint& pos) = 0;

  /// Return the identifier of the layer.
  /// \see setId();
  virtual QByteArray getId() = 0;

  /// Add the map layer to the osg group of the map layer group.
  virtual void addToScene(Core::MapLayerGroup *group) = 0;

  /// Remove the layer from the osg group of the map layer group.
  virtual void removeFromScene(Core::MapLayerGroup *group) = 0;

  /// Return the tree widget item corresponding to this map layer.
  virtual QTreeWidgetItem* getTreeItem() = 0;
  
  /// Save state of the map layer and possibly its children 
  /// \see Updraft::Core::MapLayerGroup::saveState()
  virtual QByteArray saveState() = 0;

  /// Restore the previsously saved state.
  /// \return success flag.
  /// \see saveState()
  virtual bool restoreState(const QByteArray &state) = 0;

  /// Notify the map layer that it was inserted into a group.
  virtual void inserted(Core::MapLayerGroup* parent) = 0;
  virtual void setDisabled(bool disabled) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MAPLAYERINTERFACE_H_
