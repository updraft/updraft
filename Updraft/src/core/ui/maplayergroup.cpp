#include <osg/Group>
#include <osgEarthUtil/ObjectPlacer>
#include "maplayergroup.h"
#include "../nodemaplayer.h"

namespace Updraft {
namespace Core {

MapLayerGroup::MapLayerGroup(QTreeWidget *widget, const QString &title,
    osg::Group* group, osgEarth::MapNode* map) {
  treeItem = new QTreeWidgetItem();
  treeItem->setText(0, title);
  treeItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  treeItem->setCheckState(0, Qt::Checked);
  mapLayers.insert(treeItem, NULL);
  listWidget = widget;
  nodeGroup = group;
  mapNode = map;

  connect(widget, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
          this, SLOT(itemChanged(QTreeWidgetItem*, int)));
}

MapLayerGroup::~MapLayerGroup() {
  for (TMapLayers::iterator it = mapLayers.begin();
      it != mapLayers.end(); ++it) {
    if (it.value() != NULL) it.value()->removeFromScene(this);
    // TODO(Maria): Do we also want to delete the map layer object?

    // Delete the checkbox
    treeItem->removeChild(it.key());
  }
  mapLayers.clear();

  int index = listWidget->indexOfTopLevelItem(treeItem);
  if (index != -1) {  // otherwise the item has been hidden already
    listWidget->takeTopLevelItem(index);
  }
  delete treeItem;
}

void MapLayerGroup::displayTree() {
  listWidget->addTopLevelItem(treeItem);
}

void MapLayerGroup::hideTree() {
  listWidget->takeTopLevelItem
    (listWidget->indexOfTopLevelItem(treeItem));
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osg::Node* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new NodeMapLayer(mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (MapLayerInterface* layer, const QString &title,
  int pos, QTreeWidgetItem* toTree) {
  QTreeWidgetItem *newItem = new QTreeWidgetItem();
  newItem->setText(0, title);
  newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  Qt::CheckState state;
  if (layer->isVisible()) {
    state = Qt::Checked;
  } else {
    state = Qt::Unchecked;
  }
  newItem->setCheckState(0, state);

  layer->connectSignalDisplayed
      (this, SLOT(mapLayerVisibilityChanged(bool, MapLayerInterface*)));

  // add the item into the menu list and maplist
  addIntoList(newItem, pos, toTree);
  mapLayers.insert(newItem, layer);

  // insert the node into the scene
  layer->addToScene(this);

  return layer;
}

QVector<MapLayerInterface*>* MapLayerGroup::insertMapLayerGroup
  (QVector<QPair<osg::Node*, QString> >* mapLayerGroup,
  const QString& title, int pos) {
  // Check the array
  if (mapLayerGroup == NULL)
    return NULL;
  QVector<MapLayerInterface*>* layers = new QVector<MapLayerInterface*>();

  // Add subtree
  QTreeWidgetItem *newItem = new QTreeWidgetItem();
  newItem->setText(0, title);
  newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

  newItem->setCheckState(0, Qt::Checked);

  // Add all the layers int he array
  for (int i = 0; i < mapLayerGroup->size(); ++i) {
    MapLayerInterface* layer =
      new NodeMapLayer(mapLayerGroup->at(i).first);
    QString subtitle(mapLayerGroup->at(i).second);
    layers->push_back(insertMapLayer(layer, subtitle, -1, newItem));
  }

  addIntoList(newItem, pos);
  mapLayers.insert(newItem, NULL);

  return layers;
}

MapLayerInterface* MapLayerGroup::insertExistingMapLayer
  (osg::Node* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new NodeMapLayer(mapLayer);
  return insertExistingMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertExistingMapLayer
  (MapLayerInterface* layer, const QString &title, int pos) {
  QTreeWidgetItem *newItem = new QTreeWidgetItem();
  newItem->setText(0, title);
  newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  Qt::CheckState state;
  if (layer->isVisible()) {
    state = Qt::Checked;
  } else {
    state = Qt::Unchecked;
  }
  newItem->setCheckState(0, state);

  layer->connectSignalDisplayed
      (this, SLOT(mapLayerVisibilityChanged(bool, MapLayerInterface*)));

  // add the item into the menu list and maplist
  addIntoList(newItem, pos);
  mapLayers.insert(newItem, layer);

  // don't insert the node into the scene!

  return layer;
}

void MapLayerGroup::removeMapLayer(MapLayerInterface* layer) {
  for (TMapLayers::iterator it = mapLayers.begin();
      it != mapLayers.end(); ++it) {
    if (it.value() == layer) {
      // remove the node from the scene
      layer->removeFromScene(this);

      // TODO(Maria): Do we also want to destroy the layer object?
      // Or should the plugin destroy it by itself?
      // Delete the checkbox from the list.
      treeItem->removeChild(it.key());
      // delete it.value();
      mapLayers.erase(it);
      break;
    }
  }

  if (mapLayers.count() == 1) {
    hideTree();
  }
}

void MapLayerGroup::setMapLayerTitle(MapLayerInterface* layer,
  const QString &title) {
  for (TMapLayers::iterator it = mapLayers.begin();
      it != mapLayers.end(); ++it) {
    if (it.value() == layer) {
      it.key()->setText(0, title);
      break;
    }
  }
}

osgEarth::MapNode* MapLayerGroup::getMapNode() {
  return mapNode;
}

osg::Group* MapLayerGroup::getNodeGroup() {
  return nodeGroup;
}

osgEarth::Util::ObjectPlacer* MapLayerGroup::getObjectPlacer() {
  // The boolean must be false, otherwise it doesn't work for unknown reasons.
  // This was written in a comment in OsgEarth source code.
  return new osgEarth::Util::ObjectPlacer(mapNode, 0, false);
}

void MapLayerGroup::itemChanged(QTreeWidgetItem *item, int column) {
  if (item->checkState(column) == Qt::Checked) {
    changeLayersVisibility(item, column, true);
  } else {
    changeLayersVisibility(item, column, false);
  }
}

void MapLayerGroup::changeLayersVisibility(QTreeWidgetItem *item, int column,
  bool value) {
  // Finds MapLayer object for map layer and emits the display/hide signal.
  TMapLayers::iterator it = mapLayers.find(item);
  if (it == mapLayers.end()) return;
  for (int i = 0; i < item->childCount(); i++) {
    QTreeWidgetItem* child = item->child(i);
    TMapLayers::iterator j = mapLayers.find(child);
    // if (j == mapLayers.end()) continue;
    if (j.value() == NULL) {
      child->setDisabled(!value);
      if (child->checkState(column) == Qt::Checked) {
        changeLayersVisibility(child, column, true && value);
      } else {
        changeLayersVisibility(child, column, false);
      }
    } else {
      if (value) {
        child->setDisabled(false);
        if (j.key()->checkState(column) == Qt::Checked) {
          j.value()->emitChecked(true);
        }
        // the ones that are not checked are still not visible
      } else if (!value) {
        // hide and disable everything fromthe group
        child->setDisabled(true);
        mapLayers.find(child).value()->emitChecked(false);
      }
    }
  }
  if (it.value() == NULL)
    return;
  if (value) {
    it.value()->emitChecked(true);
  } else if (!value) {
    it.value()->emitChecked(false);
  }
}

void MapLayerGroup::addIntoList(QTreeWidgetItem *item,
  int pos, QTreeWidgetItem* toTree) {
  // if this is the first layer item (there is always one for the group),
  // create the tree:
  if (mapLayers.count() == 1) {
    displayTree();
  }

  QTreeWidgetItem* branch = (toTree) ? toTree : treeItem;

  int position = 0;
  if ((pos > branch->childCount()) || (pos < 0)) {
    position = branch->childCount();
  } else {
    position = pos;
  }

  branch->insertChild(position, item);
}

void MapLayerGroup::mapLayerVisibilityChanged(bool value,
  MapLayerInterface* layer) {
  for (TMapLayers::iterator it = mapLayers.begin();
    it != mapLayers.end(); it++) {
    if (it.value() == layer) {
      Qt::CheckState state;
      if (value) {
        state = Qt::Checked;
      } else {
        state = Qt::Unchecked;
      }
      if (!(it.key()->isDisabled())) {
        it.key()->setCheckState(0, state);
      }
      break;
    }
  }
}

}  // End namespace Core
}  // End namespace Updraft
