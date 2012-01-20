#include <osg/Group>
#include "maplayergroup.h"
#include "../maplayer.h"

namespace Updraft {

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
    delete it.value();
  }
  mapLayers.clear();

  QTreeWidget *widget = treeItem->treeWidget();
  widget->takeTopLevelItem(widget->indexOfTopLevelItem(treeItem));
  delete treeItem;

  // TODO(Maria): should we delete the scene subtree? how?
}

void MapLayerGroup::displayTree() {
  listWidget->addTopLevelItem(treeItem);
}

void MapLayerGroup::hideTree() {
  listWidget->removeItemWidget(treeItem, 0);
}

int MapLayerGroup::generateID() {
  return mapLayers.size();
}

MapLayerInterface* MapLayerGroup::createEmptyMapLayer() {
  return new Core::MapLayer(generateID());
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (MapLayerType type, Layer mapLayer, const QString &title, int pos) {
  MapLayerInterface* layer = new Core::MapLayer(generateID(), type, mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osg::Node* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new Core::MapLayer(generateID(), mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osgEarth::ImageLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new Core::MapLayer(generateID(), mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osgEarth::ElevationLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new Core::MapLayer(generateID(), mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (osgEarth::ModelLayer* mapLayer, const QString& title, int pos) {
  MapLayerInterface* layer = new Core::MapLayer(generateID(), mapLayer);
  return insertMapLayer(layer, title, pos);
}

MapLayerInterface* MapLayerGroup::insertMapLayer
  (MapLayerInterface* layer, const QString &title, int pos) {
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

  QTreeWidgetItem *newItem = new QTreeWidgetItem();
  newItem->setText(0, title);
  newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  newItem->setCheckState(0, Qt::Checked);
  treeItem->insertChild(position, newItem);
  mapLayers.insert(layer, newItem);

  // insert the node into the scene
  switch (layer->getType()) {
    case MapLayerType::OSG_NODE_LAYER: {
      nodeGroup->addChild(layer->getLayer().osgNode);
      break;
    }
    case MapLayerType::IMAGE_LAYER: {
      // nodeGroup->addChild(layer->getLayer()->im);
    }
    case MapLayerType::ELEVATION_LAYER: {
      // nodeGroup->addChild(layer->getLayer()->osgNode);
    }
    case MapLayerType::MODEL_LAYER: {
      // nodeGroup->addChild(layer->getLayer()->osgNode);
    }
  }
  return layer;
}

void MapLayerGroup::removeMapLayer(MapLayerInterface* layer) {
  TMapLayers::iterator it = mapLayers.find(layer);
  if (it != mapLayers.end()) {
    delete it.value();
    mapLayers.erase(it);
  }
  if (mapLayers.empty()) {
    hideTree();
  }
}

osgEarth::MapNode* MapLayerGroup::getMapNode() {
  return mapNode;
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

}  // End namespace Updraft
