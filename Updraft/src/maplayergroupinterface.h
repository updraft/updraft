#ifndef UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_
#define UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_

#include "maplayerinterface.h"
#include <QVector>
#include <QPair>

class QTreeWidgetItem;

namespace osg {
  class Node;
  class Group;
}

namespace osgEarth {
  class MapNode;
  class ImageLayer;
  class ElevationLayer;
  class ModelLayer;

  namespace Util {
    class ObjectPlacer;
  }
}

namespace Updraft {

namespace Core {
  class MapManager;
  class SceneManager;
}

/// Map layer group used by plugins.
/// This class encapsulates handling of the map layer group.
/// It provides methods for creating and deleting layers and
/// automatically adding to/deleting from the GUI.
class MapLayerGroupInterface {
 public:
  virtual ~MapLayerGroupInterface() {}

  /// Creates an empty instance of MapLayer with generated ID.
  virtual MapLayerInterface* createEmptyMapLayer() = 0;

  /// Inserts map layer into this group.
  /// insertMapLayer adds new QTreeWidgetItem.
  /// \param pos desired position of layer in the tree view
  ///   values < 0, or bigger than number of layers already in
  ///   the group append the layer at the end.
  /// \param layer pointer to osg Node representing geometry of this layer
  /// \param title name of the layer; caption of the tree item
  virtual MapLayerInterface* insertMapLayer
    (osg::Node* layer, const QString& title, int pos = -1) = 0;
  virtual MapLayerInterface* insertMapLayer
    (osgEarth::ImageLayer* layer, const QString& title, int pos = -1) = 0;
  virtual MapLayerInterface* insertMapLayer
    (osgEarth::ElevationLayer* layer, const QString& title, int pos = -1) = 0;
  virtual MapLayerInterface* insertMapLayer
    (osgEarth::ModelLayer* layer, const QString& title, int pos = -1) = 0;
  virtual QVector<MapLayerInterface*>* insertMapLayerGroup
    (QVector<QPair<osg::Node*, QString> > * mapLayerGroup,
    const QString& title, int pos = -1) = 0;

  /// Removes map layer.
  virtual void removeMapLayer(MapLayerInterface* layer) = 0;

  /// Sets title of the map layer (displayed in the left pane).
  virtual void setMapLayerTitle(MapLayerInterface* layer, const QString &title) = 0;

  /// Creates the treeItem, without any map layer dependency.
  virtual QTreeWidgetItem* createTreeItem
    (const QString& title, int pos = -1) = 0;

  /// Removes the treeItem: but only if it doesn't have any map dependency.
  virtual void removeTreeItem(QTreeWidgetItem* item) = 0;

  /// Returns the mapNode.
  virtual osgEarth::MapNode* getMapNode() = 0;

  /// Returns the placer associated with the mapNode.
  virtual osgEarth::Util::ObjectPlacer* getObjectPlacer() = 0;

  friend class Core::SceneManager;
  friend class Core::MapManager;

 private:
  /// Functions to add map layer without adding it to scene.
  virtual MapLayerInterface* insertExistingMapLayer
    (osg::Node* layer, const QString& title, int pos = -1) = 0;
  virtual MapLayerInterface* insertExistingMapLayer
    (osgEarth::ImageLayer* layer, const QString& title, int pos = -1) = 0;
  virtual MapLayerInterface* insertExistingMapLayer
    (osgEarth::ElevationLayer* layer, const QString& title, int pos = -1) = 0;
  virtual MapLayerInterface* insertExistingMapLayer
    (osgEarth::ModelLayer* layer, const QString& title, int pos = -1) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_
