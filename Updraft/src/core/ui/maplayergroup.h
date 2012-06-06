#ifndef UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_
#define UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

#pragma warning(push)
#pragma warning(disable:4250)

#include <QtGui>
#include <osg/ref_ptr>
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
class MapLayerGroup : public MapLayer, public MapLayerGroupInterface {
  Q_OBJECT

 public:
  /// Creates new map layer group.
  /// For creating new instances use CoreInterface or for creating
  /// sub groups use MapLayerGroup::createMapLayerGroup.
  /// \param title Displayed name of the widget.
  explicit MapLayerGroup(const QString &title);

  /// Create a new map layer group based on already existing
  /// tree item.
  explicit MapLayerGroup(QTreeWidgetItem *item);

  /// Destroys MapLayerGroup
  /// Removes coresponding tree widget from GUI.
  ~MapLayerGroup();

  /// Create a single empty map layer group and add it as a child of this.
  /// \return The new group.
  MapLayerGroupInterface* createMapLayerGroup(const QString &title,
    int pos = -1);

  /// Create a single map layer containing the node and add it as a child of
  /// this group.
  /// \return The new layer.
  MapLayerInterface* createMapLayer(osg::Node* layer, const QString& title,
    int pos = -1);

  /// Insert the map layer as a child of this group.
  void insertMapLayer(MapLayerInterface* layer, int pos = -1);

  /// Remove the map layer from this group, but don't delete it.
  /// Ownership of the layer is transfered to caller of this method.
  void removeMapLayer(MapLayerInterface* layer);

  /// Returns the osg group associated with this map layer group.
  osg::Group* getNodeGroup();

  osgEarth::Util::ObjectPlacer* getObjectPlacer();

  void setVisibility(bool value);
  bool isVisible();

  /// Save the state (check state and expanded state)
  /// of this layer (and possibly all children).
  ///
  /// \note
  /// Format of the save is following:
  /// 1) For each child:
  ///   a) Save child's id
  ///   c) Save the child's data as QByteArray
  /// 3) Empty QByteArray as a separator
  /// 4) Save this as MapLayer
  QByteArray saveState();

  /// Restore the previsously saved state.
  /// \see saveState()
  bool restoreState(const QByteArray &state);

 private:
  void addToScene(MapLayerGroup* group);
  void removeFromScene(MapLayerGroup* group);
  void inserted(Core::MapLayerGroup* parent);

  void setDisabled(bool disabled);

  QSet<MapLayerInterface*> mapLayers;

  /// Pointer to the subtree of the scene associated with this layer.
  osg::ref_ptr<osg::Group> nodeGroup;

  MapLayerInterface* createMapLayerNoInsert
    (osg::Node* mapLayer, const QString& title, int pos = -1);
};

}  // End namespace Core
}  // End namespace Updraft

#pragma warning(pop)  // C2450

#endif  // UPDRAFT_SRC_CORE_UI_MAPLAYERGROUP_H_

