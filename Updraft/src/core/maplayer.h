#ifndef UPDRAFT_SRC_CORE_MAPLAYER_H_
#define UPDRAFT_SRC_CORE_MAPLAYER_H_

#include <QObject>
#include <QPoint>
#include "../maplayerinterface.h"

class QTreeWidgetItem;

namespace osg {
class Node;
}

namespace Updraft {
namespace Core {

class MapLayerGroup;

/// A class that represents one logical layer in map.
/// Map layer in osgEarth can be of different types, and this
/// class encapsulates all of the types, and creates a common
/// interface.
/// The type of the map layer determines which value of the
/// union Layer is valid.
class MapLayer : public QObject, virtual public MapLayerInterface {
  Q_OBJECT

 public:
  explicit MapLayer(const QString &title);
  ~MapLayer();

  /// Connects the checkbox signal to a given slot
  /// \param receiver The object on which the slot should be called
  /// \param method The slot that should be called
  void connectSignalChecked(const QObject* receiver,
    const char *method);

  /// Connects the context menu signal to a given slot
  /// \param receiver The object on which the slot should be called
  /// \param method The slot that should be called
  void connectSignalContextMenuRequested(const QObject* receiver,
    const char* method);

  /// Connects the given signal to the slot that sets visibility of this
  /// map layer.
  /// \param sender The object whose signal should be connected to the
  ///               visibility toggling slot.
  /// \param method The signal that should be connected
  void connectSlotSetVisibility(const QObject* sender,
    const char *method);

  /// A convenience method that connects the checked signal of this map layer.
  /// directly to its visiblity toggling slot.
  void connectCheckedToVisibility();

  /// Sets the identification of the map layer.
  /// \param id The id in the local 8bit encoding, identifying the map layer.
  void setId(const QByteArray& id);

  /// Returns this map layer's id.
  /// \return The id in local 8-bit encoding. This is usually the title string.
  QByteArray getId();

  /// Sets the title of this map layer.
  /// \param title The new title to be set.
  void setTitle(const QString& title);

  /// Returns the root tree item that represents this map layer.
  /// \return pointer to this layer's tree item.
  QTreeWidgetItem *getTreeItem();

  /// Disables or enables the map layer.
  /// \param disabled Whether the layer should be disabled.
  void setDisabled(bool disabled);

  /// Checks or unchecks the map layer's check-box.
  /// \param value The new check state of the check-box.
  void setChecked(bool value);

  /// Enables or disables the checkability of the map layer's check-box.
  /// \param value Whether the check-box should be checkable.
  void setCheckable(bool value);

  /// Sets the parent map layer group of this map layer.
  /// \param parent The new parent group of this map layer.
  void inserted(MapLayerGroup* parent);

  /// Sets the file path of the file associated to this layer to the given
  /// string.
  /// \param path The path to the associated file
  void setFilePath(const QString& path);

  /// Returns pointer to the action that deletes the associated file.
  /// \return The QAction that deletes the file associated to this map layer.
  QAction* getDeleteAction();

  /// Save the state (check state and expanded state)
  /// of this layer.
  /// \return QByteArray with the saved state of this map layer.
  QByteArray saveState();

  /// Restore the previsously saved state.
  /// \param state The state that was previously obtained from the saveState()
  ///              method.
  /// \see saveState()
  bool restoreState(const QByteArray &state);

 public slots:
  /// Displays/hides the map layer.
  /// \param value The new state of visibility.
  void setVisibility(bool value) = 0;

  void zoom();

 private slots:
  /// The slot that is executed by the delete action. This method deleted the
  /// associated file.
  void deleteActionSlot();

 signals:
  /// Emited when the map layer is checked or unchecked
  /// (before it is displayed or hidden).
  /// \param value Whether the layer was checked
  /// \param sender The pointer to this object
  void checked(bool value, MapLayerInterface* sender);

  /// Emited when a context menu is requested on this map layer.
  /// \param pos The position where the context menu should appear.
  /// \param sender Pointer to this object.
  void contextMenuRequested(QPoint pos, MapLayerInterface* sender);

 protected:
  /// Return the osg node of this map layer or null if there is none.
  virtual const osg::Node* getNode() = 0;

  /// Emits the checked signal.
  /// Called from MainWindow.
  /// \param value Whether the check-box was checked
  void emitChecked(bool value);

  /// Emits the context menu request signal.
  /// \param pos The position of the context menu.
  void emitContextMenuRequested(const QPoint& pos);

  explicit MapLayer(QTreeWidgetItem* item);

  /// Identificiation of the map layer, used in saving and restoring state.
  /// by default the id is the title encoded to local 8bit encoding.
  QByteArray id;

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
