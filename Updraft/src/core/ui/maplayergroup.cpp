#include <osg/Group>
#include "maplayergroup.h"
#include "../maplayer.h"

namespace Updraft {

  MapLayerGroup::MapLayerGroup(QTreeWidget *widget, const QString &title,
    osg::Group* group, osgEarth::MapNode* map) {
  treeItem = new QTreeWidgetItem();
  treeItem->setText(0, title);
  widget->addTopLevelItem(treeItem);
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

MapLayerInterface* MapLayerGroup::createMapLayer() {
  return new Core::MapLayer();
}

void MapLayerGroup::insertMapLayer(int pos,
                                    MapLayerInterface* layer,
                                    const QString &title) {
  QTreeWidgetItem *newItem = new QTreeWidgetItem();
  newItem->setText(0, title);
  newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  newItem->setCheckState(0, Qt::Checked);
  treeItem->insertChild(pos, newItem);
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
}

void MapLayerGroup::removeMapLayer(MapLayerInterface* layer) {
  TMapLayers::iterator it = mapLayers.find(layer);
  if (it != mapLayers.end()) {
    delete it.value();
    mapLayers.erase(it);
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
