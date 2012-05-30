#ifndef UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_
#define UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_

#include "maplayerinterface.h"
#include <QVector>
#include <QPair>

namespace osg {
  class Node;
}

namespace osgEarth {
  class MapNode;

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

  /// Inserts map layer into this group.
  /// insertMapLayer adds new QTreeWidgetItem.
  /// \param pos Desired position of layer in the tree view
  ///   values < 0, or bigger than number of layers already in
  ///   the group append the layer at the end.
  /// \param layer Pointer representing this layer.
  /// \param title Name of the layer; caption of the tree item.
  virtual MapLayerInterface* insertMapLayer
    (osg::Node* layer, const QString& title, int pos = -1) = 0;

  /// Insert map layer group to the map.
  /// insertMapLayer adds new QTreeWidgetItem.
  /// \param pos desired position of layer in the tree view
  ///   values < 0, or bigger than number of layers already in
  ///   the group append the layer at the end.
  /// \param mapLayerGroup List of nodes and its titles.
  /// \param title name of the layer; caption of the tree item
  virtual QVector<MapLayerInterface*>* insertMapLayerGroup
    (QVector<QPair<osg::Node*, QString> > * mapLayerGroup,
    const QString& title, int pos = -1) = 0;

  /// Removes map layer.
  virtual void removeMapLayer(MapLayerInterface* layer) = 0;

  /// Sets title of the map layer (displayed in the left pane).
  virtual void setMapLayerTitle(MapLayerInterface* layer, const QString &title) = 0;

  /// Returns the mapNode.
  virtual osgEarth::MapNode* getMapNode() = 0;

  /// Returns the placer associated with the mapNode.
  virtual osgEarth::Util::ObjectPlacer* getObjectPlacer() = 0;

  friend class Core::SceneManager;
  friend class Core::MapManager;

 private:
  /// Add map layer without adding it to scene.
  virtual MapLayerInterface* insertExistingMapLayer
    (osg::Node* layer, const QString& title, int pos = -1) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_
