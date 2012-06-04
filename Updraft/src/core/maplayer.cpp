#include "maplayer.h"

#include <QTreeWidgetItem>
#include <QFile>

#include "ui/maplayergroup.h"

namespace Updraft {
namespace Core {

MapLayer::MapLayer(const QString &title)
  : deleteAction(0), ownsTreeItem(true), parent(0) {
  treeItem = new QTreeWidgetItem();
  treeItem->setText(0, title);
  treeItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  treeItem->setCheckState(0, Qt::Checked);
}

MapLayer::MapLayer(QTreeWidgetItem *item)
  : deleteAction(0), treeItem(item), ownsTreeItem(false), parent(0) {}

MapLayer::~MapLayer() {
  if (ownsTreeItem) {
    delete treeItem;
  }
}

void MapLayer::connectSignalChecked(const QObject *receiver,
  const char *method) {
  connect(this, SIGNAL(checked(bool, MapLayerInterface*)), receiver, method);
}

void MapLayer::connectSignalContextMenuRequested(const QObject* receiver,
  const char* method) {
  connect(this, SIGNAL(contextMenuRequested(QPoint, MapLayerInterface*)),
    receiver, method);
}

void MapLayer::connectSlotSetVisibility(const QObject *sender,
  const char* method) {
  connect(sender, method, this, SLOT(setVisibility(bool)));
}

void MapLayer::connectCheckedToVisibility() {
  connect(this, SIGNAL(checked(bool, MapLayerInterface*)),
    this, SLOT(setVisibility(bool)));
}

QTreeWidgetItem *MapLayer::getTreeItem() {
  return treeItem;
}

void MapLayer::inserted(MapLayerGroup* parent) {
  this->parent = parent;
}

void MapLayer::setDisabled(bool disabled) {
  treeItem->setDisabled(disabled);
}

void MapLayer::setTitle(const QString& title) {
  treeItem->setText(0, title);
}

void MapLayer::setChecked(bool value) {
  getTreeItem()->setCheckState(0, value ? Qt::Checked : Qt::Unchecked);
}

void MapLayer::setCheckable(bool value) {
  if (value) {
    getTreeItem()->setFlags(getTreeItem()->flags() | Qt::ItemIsUserCheckable);
    getTreeItem()->setCheckState(0, Qt::Checked);
  } else {
    getTreeItem()->setFlags(getTreeItem()->flags() & ~Qt::ItemIsUserCheckable);
    getTreeItem()->setData(0, Qt::CheckStateRole, QVariant());
  }
}

void MapLayer::emitChecked(bool state) {
  emit checked(state, this);
}

void MapLayer::emitContextMenuRequested(const QPoint& pos) {
  emit contextMenuRequested(pos, this);
}

void MapLayer::setFilePath(const QString& path) {
  deleteFilePath = path;
}

QAction* MapLayer::getDeleteAction() {
  if (!deleteAction) {
    deleteAction = new QAction(tr("Delete"), this);
    connect(deleteAction, SIGNAL(triggered()),
      this, SLOT(deleteActionSlot()));
  }
  return deleteAction;
}

void MapLayer::deleteActionSlot() {
  QFile::remove(deleteFilePath);
  delete this;
}

}  // End namespace Core
}  // End namespace Updraft
