#include "maplayer.h"

#include <QTreeWidgetItem>
#include <QFile>

#include <osgEarth/Viewpoint>
#include <osg/BoundingSphere>

#include "ui/maplayergroup.h"
#include "updraft.h"
#include "scenemanager.h"

namespace Updraft {
namespace Core {

MapLayer::MapLayer(const QString &title)
  : deleteAction(0), zoomAction(0), ownsTreeItem(true), parent(0) {
  treeItem = new QTreeWidgetItem();
  treeItem->setText(0, title);
  treeItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  treeItem->setCheckState(0, Qt::Checked);

  id = title.toLocal8Bit();
}

MapLayer::MapLayer(QTreeWidgetItem *item)
  : deleteAction(0), zoomAction(0),
  treeItem(item), ownsTreeItem(false), parent(0) {}

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
  deleteFilePath = updraft->getDataDirectory().relativeFilePath(path);
}

QAction* MapLayer::getDeleteAction() {
  if (!deleteAction) {
    deleteAction = new QAction(tr("Delete"), this);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteActionSlot()));
  }
  return deleteAction;
}

QAction* MapLayer::getZoomAction() {
  if (!zoomAction) {
    zoomAction = new QAction(tr("Zoom"), this);
    connect(zoomAction, SIGNAL(triggered()), this, SLOT(zoom()));
  }
  return zoomAction;
}

void MapLayer::deleteActionSlot() {
  QFile::remove(updraft->getDataDirectory().filePath(deleteFilePath));
  delete this;
}

void MapLayer::zoom() {
  const osg::BoundingSphere& bound = getNode()->getBound();

  if (bound.radius() < 0) {
    return;
  }

  osg::Vec3d centerLatLon;

  updraft->sceneManager->getCurrentMapEllipsoid()->convertXYZToLatLongHeight(
    bound.center().x(), bound.center().y(), bound.center().z(),
    centerLatLon.y(), centerLatLon.x(), centerLatLon.z());

  centerLatLon.x() = osg::RadiansToDegrees(centerLatLon.x());
  centerLatLon.y() = osg::RadiansToDegrees(centerLatLon.y());

  osgEarth::Viewpoint viewpoint(centerLatLon,
    0, -90, bound.radius() * 3.8);
    // Magical constant -- a little more than cotg(15 deg).
    // This means that with field of view of 30 degrees
    // the bounding sphere will contain a little less than the whole
    // screen.

  updraft->sceneManager->animateToViewpoint(viewpoint);
}

void MapLayer::setId(const QByteArray& id) {
  this->id = id;
}

QByteArray MapLayer::getId() {
  return id;
}

QByteArray MapLayer::saveState() {
  QByteArray array;
  QDataStream stream(&array, QIODevice::WriteOnly);

  stream << treeItem->isExpanded();
  stream << (treeItem->checkState(0) == Qt::Checked);

  return array;
}

bool MapLayer::restoreState(const QByteArray& state) {
  QDataStream stream(state);

  bool expanded;
  stream >> expanded;
  treeItem->setExpanded(expanded);

  bool checked;
  stream >> checked;

  // CheckStateRole data not valid means that there is no checkbox
  // and we don't want to add one.
  if (treeItem->data(0, Qt::CheckStateRole).isValid()) {
    treeItem->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
  }

  return true;
}

}  // End namespace Core
}  // End namespace Updraft
