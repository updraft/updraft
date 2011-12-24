#ifndef UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_
#define UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

#include <QtGui>

namespace osg {
  class Node;
}

namespace Updraft {

/// Map layer group used by plugins.
/// This class encapsulates handling of the map layer group.
/// It provides methods for creating and deleting layers and
/// automatically adding to/deleting from the GUI.
class MapLayerGroup : public QObject {
  Q_OBJECT

 public:
  /// Creates new map layer group in the target tree widget.
  /// For creating new instances use CoreInterface.
  /// \param widget pointer to the tree widget
  MapLayerGroup(QTreeWidget *widget, const QString &title);

  /// Destroys MapLayerGroup
  /// Removes coresponding tree widget from GUI.
  virtual ~MapLayerGroup();

  /// Inserts map layer into this group.
  /// insertMapLayer adds new QTreeWidgetItem and connects
  /// it's signal itemchanged to slot MapLayerGroup::itemChanged.
  /// Map layer is identified by pointer to osg::Node.
  /// \param pos desired position of layer in the tree view
  /// \param layer pointer to osg Node representing geometry of this layer
  /// \param title name of the layer; caption of the tree item
  virtual void insertMapLayer(int pos, osg::Node* layer, const QString &title);

  /// Removes the map layer from scene and from the tree view.
  /// \param layer pointer to node which will be removed
  virtual void removeMapLayer(osg::Node* layer);

 signals:
  /// Called when layer is displayed.
  /// \param layer pointer to node which visibility has been changed
  void mapLayerDisplayed(osg::Node* layer);

  /// Called when layer is hidden.
  /// \param layer pointer to node which visibility has been changed
  void mapLayerHidden(osg::Node* layer);

 private slots:
  /// Called when a map layer is about to be displayed/hiden.
  /// \param item pointer to QTreeWidgetItem item which have been changed
  /// \param column horizontal position in tree view (not used)
  virtual void itemChanged(QTreeWidgetItem *item, int column);

 private:
  QTreeWidgetItem *treeItem;
  typedef QMap<osg::Node*, QTreeWidgetItem*> TMapLayers;
  TMapLayers mapLayers;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

