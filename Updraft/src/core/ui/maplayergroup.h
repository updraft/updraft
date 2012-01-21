#ifndef UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_
#define UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

#include <QtGui>
#include "../../maplayerinterface.h"

namespace osg {
  class Node;
  class Group;
}

namespace osgEarth {
  class MapNode;
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
  /// \param nodeGroup pointer to scene subtree of this layer group
  MapLayerGroup(QTreeWidget *widget, const QString &grouptitle,
    osg::Group* nodeGroup, osgEarth::MapNode* map);

  /// Destroys MapLayerGroup
  /// Removes coresponding tree widget from GUI.
  virtual ~MapLayerGroup();

  /// Creates and inserts a new instance of Map Layer with generated ID
  virtual MapLayerInterface* createEmptyMapLayer();

  /// Creates and inserts a new instance of Map Layer with generated ID,
  /// and filled values.
  virtual MapLayerInterface* insertMapLayer
    (MapLayerType type, Layer layer, const QString& title, int pos = -1);

  /// Creates and inserts a new instance of Map Layer with generated ID,
  /// and filled values.
  virtual MapLayerInterface* insertMapLayer
    (osg::Node* layer, const QString& title, int pos = -1);

  /// Creates and inserts a new instance of Map Layer with generated ID,
  /// and filled values.
  virtual MapLayerInterface* insertMapLayer
    (osgEarth::ImageLayer* layer, const QString& title, int pos = -1);

  /// Creates and inserts a new instance of Map Layer with generated ID,
  /// and filled values.
  virtual MapLayerInterface* insertMapLayer
    (osgEarth::ElevationLayer* layer, const QString& title, int pos = -1);

  /// Creates and inserts a new instance of Map Layer with generated ID,
  /// and filled values.
  virtual MapLayerInterface* insertMapLayer
    (osgEarth::ModelLayer* layer, const QString& title, int pos = -1);

  /// Inserts map layer into this group.
  /// insertMapLayer adds new QTreeWidgetItem and connects
  /// it's signal itemchanged to slot MapLayerGroup::itemChanged.
  /// Map layer is identified by pointer to osg::Node.
  /// \param pos desired position of layer in the tree view
  /// \param layer pointer to osg Node representing geometry of this layer
  /// \param title name of the layer; caption of the tree item
  virtual MapLayerInterface* insertMapLayer(MapLayerInterface* layer,
    const QString &title, int pos = -1);

  /// Removes the map layer from scene and from the tree view.
  /// \param layer pointer to node which will be removed
  virtual void removeMapLayer(MapLayerInterface* layer);

  /// Returns the map node.
  virtual osgEarth::MapNode* getMapNode();

 signals:
  /// Called when layer is displayed.
  /// \param layer pointer to node which visibility has been changed
  void mapLayerDisplayed(osg::Node* layer);

  /// Called when layer is hidden.
  /// \param layer pointer to node which visibility has been changed
  void mapLayerHidden(osg::Node* layer);

 private slots:
  /// Called when a map layer is about to be displayed/hidden.
  /// \param item pointer to QTreeWidgetItem item which have been changed
  /// \param column horizontal position in tree view (not used)
  virtual void itemChanged(QTreeWidgetItem *item, int column);

 private:
  /// widget to draw the list of the map layers
  QTreeWidget* listWidget;
  /// top level item
  QTreeWidgetItem *treeItem;
  typedef QMap<MapLayerInterface*, QTreeWidgetItem*> TMapLayers;
  TMapLayers mapLayers;

  /// Pointer to the subtree of the scene associated with this layer.
  osg::Group* nodeGroup;
  osgEarth::MapNode* mapNode;

  /// Creates the TreeWidgetItem once the first map layer is added.
  void displayTree();
  void hideTree();

  int generateID();
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

