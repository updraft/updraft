#ifndef UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_
#define UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

#include <QtGui>
#include "../../maplayergroupinterface.h"
#include "../maplayer.h"

namespace osg {
  class Group;
}

namespace Updraft {
namespace Core {

/// Map layer group used by plugins.
/// This class encapsulates handling of the map layer group.
/// It provides methods for creating and deleting layers and
/// automatically adding to/deleting from the GUI.
class MapLayerGroup : public QObject, public MapLayerGroupInterface {
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
  ~MapLayerGroup();

  MapLayerInterface* insertMapLayer
    (osg::Node* layer, const QString& title, int pos = -1);

  QVector<MapLayerInterface*>* insertMapLayerGroup
    (QVector<QPair<osg::Node*, QString> >* mapLayerGroup,
    const QString& title, int pos = -1);

  void removeMapLayer(MapLayerInterface* layer);

  void setMapLayerTitle(MapLayerInterface* layer, const QString &title);

  osg::Group* getNodeGroup();
  osgEarth::MapNode* getMapNode();
  osgEarth::Util::ObjectPlacer* getObjectPlacer();

 private slots:
  /// Called when a map layer is about to be displayed/hidden.
  /// \param item pointer to QTreeWidgetItem item which have been changed
  /// \param column horizontal position in tree view (not used)
  virtual void itemChanged(QTreeWidgetItem *item, int column);

  /// if any map layer emmited the signal it was changed
  virtual void mapLayerVisibilityChanged(bool value,
    MapLayerInterface* layer);

 private:
  void changeLayersVisibility(QTreeWidgetItem *item, int column,
    bool value);

  /// widget to draw the list of the map layers
  QTreeWidget* listWidget;
  /// top level item
  QTreeWidgetItem *treeItem;
  typedef QMap<QTreeWidgetItem*, MapLayerInterface*> TMapLayers;
  TMapLayers mapLayers;

  /// Pointer to the subtree of the scene associated with this layer.
  osg::Group* nodeGroup;
  osgEarth::MapNode* mapNode;

  /// Creates the TreeWidgetItem once the first map layer is added.
  void displayTree();
  void hideTree();

  /// Inserts the map layer.
  /// This function is called by all the other public inserting functions,
  /// that are just interface.
  MapLayerInterface* insertMapLayer
    (MapLayerInterface* layer, const QString& title,
    int pos = -1, QTreeWidgetItem* toTree = NULL);

  /// Inserts the group of layers into the tree
  MapLayerInterface* insertMapLayerGroup
    (QVector<MapLayerInterface*>* layerGroup,
    const QString& title, int pos = -1);

  /// Adds the tree item into the list at given position.
  /// \param pos The position at the list.
  /// Value < 0 or > list.Length means to add the item at the end.
  void addIntoList(QTreeWidgetItem* item,
    int pos, QTreeWidgetItem* toTree = NULL);

  /// Functions to add map layer without adding it to scene.
  MapLayerInterface* insertExistingMapLayer
    (MapLayerInterface* layer, const QString& title, int pos = -1);
  MapLayerInterface* insertExistingMapLayer
    (osg::Node* layer, const QString& title, int pos = -1);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

