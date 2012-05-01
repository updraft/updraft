#include <osg/Group>
#include <osgEarthUtil/ObjectPlacer>
#include "maplayergroup.h"
#include "../maplayer.h"

namespace Updraft {
namespace Core {

MapLayerGroup::MapLayerGroup(QTreeWidget *widget, const QString &title,
    osg::Group* group, osgEarth::MapNode* map) {
  treeItem = new QTreeWidgetItem();
  treeItem->setText(0, title);
  listWidget = widget;
  nodeGroup = group;
  mapNode = map;

  connect(widget, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
          this, SLOT(itemChanged(QTreeWidgetItem*, int)));
}

MapLayerGroup::~MapLayerGroup() {
  for (TMapLayers::iterator it = mapLayers.begin();
      it != mapLayers.end(); ++it) {
    if (it.value() != NULL) removeFromScene(it.value());
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

MapLayerInterface* MapLayerGroup::createEmptyMapLayer() {
  return new Core::MapLayer();
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osg::Node* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osgEarth::ImageLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osgEarth::ElevationLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osgEarth::ModelLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
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
  addIntoScene(layer);

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
      new MapLayer(mapLayerGroup->at(i).first);
    QString subtitle(mapLayerGroup->at(i).second);
    layers->push_back(insertMapLayer(layer, subtitle, -1, newItem));
  }

  addIntoList(newItem, pos);
  mapLayers.insert(newItem, NULL);

  return layers;
}

MapLayerInterface* MapLayerGroup::insertExistingMapLayer
  (osg::Node* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
  return insertExistingMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertExistingMapLayer
  (osgEarth::ImageLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
  return insertExistingMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertExistingMapLayer
  (osgEarth::ElevationLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
  return insertExistingMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertExistingMapLayer
  (osgEarth::ModelLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new MapLayer(mapLayer);
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

  // add the item into the menu list and maplist
  addIntoList(newItem, pos);
  mapLayers.insert(newItem, layer);

  // don't insert the node into the scene!

  return layer;
}

void MapLayerGroup::addIntoScene(MapLayerInterface* layer) {
  switch (layer->getType()) {
    case OSG_NODE_LAYER:
      nodeGroup->addChild(layer->getLayer().osgNode);
      break;
    case IMAGE_LAYER:
      mapNode->getMap()->addImageLayer(layer->getLayer().imageLayer);
      break;
    case ELEVATION_LAYER:
      mapNode->getMap()->addElevationLayer(layer->getLayer().elevationLayer);
      break;
    case MODEL_LAYER:
      mapNode->getMap()->addModelLayer(layer->getLayer().modelLayer);
      break;
  }
}

void MapLayerGroup::removeFromScene(MapLayerInterface* layer) {
  switch (layer->getType()) {
    case OSG_NODE_LAYER:
      nodeGroup->removeChild(layer->getLayer().osgNode);
      break;
    case IMAGE_LAYER:
      mapNode->getMap()->removeImageLayer(layer->getLayer().imageLayer);
      break;
    case ELEVATION_LAYER:
      mapNode->getMap()->removeElevationLayer(layer->getLayer().elevationLayer);
      break;
    case MODEL_LAYER:
      mapNode->getMap()->removeModelLayer(layer->getLayer().modelLayer);
      break;
  }
}

void MapLayerGroup::removeMapLayer(MapLayerInterface* layer) {
  for (TMapLayers::iterator it = mapLayers.begin();
      it != mapLayers.end(); ++it) {
    if (it.value() == layer) {
      // remove the node from the scene
      removeFromScene(layer);

      // TODO(Maria): Do we also want to destroy the layer object?
      // Or should the plugin destroy it by itself?
      // Delete the checkbox from the list.
      treeItem->removeChild(it.key());
      // delete it.value();
      mapLayers.erase(it);
      break;
    }
  }

  if (mapLayers.empty()) {
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

QTreeWidgetItem* MapLayerGroup::createTreeItem
  (const QString& title, int pos) {
  QTreeWidgetItem* item = new QTreeWidgetItem();
  item->setText(0, title);
  item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  item->setCheckState(0, Qt::Unchecked);

  // add the item into the menu list and maplist
  addIntoList(item, pos);

  return item;
}

void MapLayerGroup::removeTreeItem(QTreeWidgetItem* item) {
  treeItem->removeChild(item);
  // TODO(Maria): probably do not destroy the object, or?
}

osgEarth::MapNode* MapLayerGroup::getMapNode() {
  return mapNode;
}

osgEarth::Util::ObjectPlacer* MapLayerGroup::getObjectPlacer() {
  // The boolean must be false, otherwise it doesn't work for unknown reasons.
  // This was written in a comment in OsgEarth source code.
  return new osgEarth::Util::ObjectPlacer(mapNode, 0, false);
}

void MapLayerGroup::itemChanged(QTreeWidgetItem *item, int column) {
  // Finds MapLayer object for map layer and emits the display/hide signal.
  TMapLayers::iterator it = mapLayers.find(item);
  if (it == mapLayers.end()) return;
  for (int i = 0; i < item->childCount(); i++) {
    QTreeWidgetItem* child = item->child(i);
    TMapLayers::iterator j = mapLayers.find(child);
    if (item->checkState(column) == Qt::Checked) {
      child->setDisabled(false);
      if (j.key()->checkState(column) == Qt::Checked) {
        j.value()->emitChecked(true);
      }
      // the ones that are not checked are still not visible
    } else if (item->checkState(column) == Qt::Unchecked) {
      // hide and disable everything fromthe group
      child->setDisabled(true);
      mapLayers.find(child).value()->emitChecked(false);
    }
  }
  if (it.value() == NULL)
    return;
  if (item->checkState(column) == Qt::Checked) {
    it.value()->emitChecked(true);
  } else if (item->checkState(column) == Qt::Unchecked) {
    it.value()->emitChecked(false);
  }
}

void MapLayerGroup::addIntoList(QTreeWidgetItem *item,
  int pos, QTreeWidgetItem* toTree) {
  // if this is the first item, create the tree:
  if (mapLayers.empty()) {
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
