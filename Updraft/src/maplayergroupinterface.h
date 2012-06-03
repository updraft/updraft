#ifndef UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_
#define UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_

#include "maplayerinterface.h"
#include <QVector>
#include <QPair>

namespace osg {
  class Node;
  class Group;
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
class MapLayerGroupInterface: virtual public MapLayerInterface {
 public:
  virtual ~MapLayerGroupInterface() {}

  /// Create a map layer group and add it as a sub item to this.
  /// To remove MapLayerGroup use C++ delete.
  /// \param title label for the item in the tree view
  /// \return Pointer to the new MapLayerGroup
  virtual MapLayerGroupInterface* createMapLayerGroup(const QString &title,
    int pos = -1) = 0;

  /// Create a new map layer containing the node and add it as a child of
  /// this group.
  /// \return The new layer.
  virtual MapLayerInterface* createMapLayer(osg::Node* layer,
    const QString& title, int pos = -1) = 0;

  /// Insert the map layer as a child of this group.
  virtual void insertMapLayer(MapLayerInterface* layer, int pos = -1) = 0;

  /// Remove the map layer from this group, but don't delete it.
  /// Ownership of the layer is transfered to caller of this method.
  virtual void removeMapLayer(MapLayerInterface* layer) = 0;

  /// Returns the osg group associated with this map layer group.
  virtual osg::Group* getNodeGroup() = 0;

  /// Returns the placer associated with the mapNode.
  virtual osgEarth::Util::ObjectPlacer* getObjectPlacer() = 0;

  friend class Core::SceneManager;
  friend class Core::MapManager;

 private:
  // TODO(Kuba): Get rid of this method.
  /// Add map layer without adding it to scene.
  virtual MapLayerInterface* createMapLayerNoInsert
    (osg::Node* layer, const QString& title, int pos = -1) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MAPLAYERGROUPINTERFACE_H_
