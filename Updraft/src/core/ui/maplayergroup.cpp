#include "maplayergroup.h"

namespace Updraft {

MapLayerGroup::MapLayerGroup(QTreeWidget *widget, const QString &title) {
  treeItem = new QTreeWidgetItem();
  treeItem->setText(0, title);
  widget->addTopLevelItem(treeItem);

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
}

void MapLayerGroup::insertMapLayer(int pos,
                                    osg::Node* layer,
                                    const QString &title) {
  QTreeWidgetItem *newItem = new QTreeWidgetItem();
  newItem->setText(0, title);
  newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  newItem->setCheckState(0, Qt::Checked);
  treeItem->insertChild(pos, newItem);
  mapLayers.insert(layer, newItem);
}

void MapLayerGroup::removeMapLayer(osg::Node* layer) {
  TMapLayers::iterator it = mapLayers.find(layer);
  if (it != mapLayers.end()) {
    delete it.value();
    mapLayers.erase(it);
  }
}

void MapLayerGroup::itemChanged(QTreeWidgetItem *item, int column) {
  // Finds osg::Node for map layer and emmits the display/hide signal.
  for (TMapLayers::iterator it = mapLayers.begin();
      it != mapLayers.end(); ++it) {
    if (it.value() == item) {
      if (item->checkState(column) == Qt::Checked) {
        emit mapLayerDisplayed(it.key());
      } else if (item->checkState(column) == Qt::Unchecked) {
        emit mapLayerHidden(it.key());
      }

      break;
    }
  }
}

}  // End namespace Updraft
