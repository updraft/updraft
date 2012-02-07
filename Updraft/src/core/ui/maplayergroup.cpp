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
    removeFromScene(it.key());

    // TODO(Maria): Do we also want to delete the map layer object?
    // Delete the checkbox
    treeItem->removeChild(it.value());
    // delete it.value();
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
  (MapLayerType type, Layer mapLayer, const QString &title, int pos) {
  MapLayerInterface* layer = new MapLayer(type, mapLayer);
  return insertMapLayer(layer, title, pos);
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
  (MapLayerInterface* layer, const QString &title, int pos) {
  QTreeWidgetItem *newItem = new QTreeWidgetItem();
  newItem->setText(0, title);
  newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  newItem->setCheckState(0, Qt::Checked);

  // add the item into the menu list and maplist
  addIntoList(newItem, pos);
  mapLayers.insert(layer, newItem);

  // insert the node into the scene
  addIntoScene(layer);

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
  TMapLayers::iterator it = mapLayers.find(layer);
  if (it != mapLayers.end()) {
    // remove the node from the scene
    removeFromScene(layer);

    // TODO(Maria): Do we also want to destroy the layer object?
    // Or should the plugin destroy it by itself?
    // Delete the checkbox from the list.
    treeItem->removeChild(it.value());
    // delete it.value();
    mapLayers.erase(it);
  }
  if (mapLayers.empty()) {
    hideTree();
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
  for (TMapLayers::iterator it = mapLayers.begin();
      it != mapLayers.end(); ++it) {
    if (it.value() == item) {
      if (item->checkState(column) == Qt::Checked) {
        emit it.key()->emitDisplayed(true);
      } else if (item->checkState(column) == Qt::Unchecked) {
        emit it.key()->emitDisplayed(false);
      }
      break;
    }
  }
}

void MapLayerGroup::addIntoList(QTreeWidgetItem *item, int pos) {
  // if this is the first item, create the tree:
  if (mapLayers.empty()) {
    displayTree();
  }

  int position = 0;
  if ((pos > treeItem->childCount()) || (pos < 0)) {
    position = treeItem->childCount();
  } else {
    position = pos;
  }

  treeItem->insertChild(position, item);
}

}  // End namespace Core
}  // End namespace Updraft
