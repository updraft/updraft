#include "settingsmodel.h"
#include "variantfunctions.h"

// TODO(cestmir): Conformance of the settings file to a DTD should be checked.

namespace Updraft {
namespace Core {

SettingsItem::SettingsItem(QDomNode node, SettingsModel* model)
  :domNode(node),
  parent(NULL),
  myModel(model) {}

SettingsItem::~SettingsItem() {
  for (int i = 0; i < children.size(); ++i) {
    delete children[i];
  }
}

void SettingsItem::prependChild(SettingsItem* item) {
  children.push_front(item);
  item->parent = this;
}

void SettingsItem::appendChild(SettingsItem* item) {
  children.push_back(item);
  item->parent = this;
}

void SettingsItem::insertChild(SettingsItem* item, int index) {
  children.insert(index, 1, item);
  item->parent = this;
}

SettingsItem* SettingsItem::getChild(int index) {
  return children.value(index, NULL);
}

int SettingsItem::childIndex(SettingsItem* item) {
  return children.indexOf(item);
}

int SettingsItem::rowCount() {
  return children.size();
}

SettingsModel::SettingsModel(): domDoc("settings") {
  QDomElement model = domDoc.createElement("model");
  domDoc.appendChild(model);
  rootItem = new SettingsItem(model, this);
}

SettingsModel::~SettingsModel() {
  if (rootItem) delete rootItem;
}

void SettingsModel::loadSettings(QString filename) {
  qDebug() << "Loading settings from " << filename;

  QFile file(filename);
  bool result;

  result = file.open(QIODevice::ReadOnly);
  if (!result) {
    file.close();
    qDebug() << "Could not load settings file!";
    return;
  }

  result = domDoc.setContent(&file);
  if (!result) {
    file.close();
    qDebug() << "Could not load settings into the application!";
    return;
  }

  // Traverse the dom document tree and create the internal item structure
  if (rootItem) delete rootItem;

  // Find the model tag
  QDomNode modelNode = domDoc.elementsByTagName("model").item(0);
  rootItem = new SettingsItem(modelNode, this);

  QStack<SettingsItem*> dfsStack;
  dfsStack.push(rootItem);

  while (!dfsStack.empty()) {
    SettingsItem* item = dfsStack.pop();
    QDomNode node = item->getNode();
    QDomNode child = node.lastChild();

    while (!child.isNull()) {
      // only traverse the <item> tags
      if (child.isElement() && child.toElement().tagName() == "item") {
        SettingsItem* childItem = new SettingsItem(child, this);
        item->prependChild(childItem);
        dfsStack.push(childItem);
      }
      child = child.previousSibling();
    }
  }
}

void SettingsModel::saveSettings(QString filename) {
  qDebug() << "Saving settings to " << filename;

  QFile file(filename);
  bool result;

  // TODO(cestmir): We might do a backup copy before truncating the file here.
  result = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
  if (!result) {
    qDebug() << "Could not open settings file for writing!";
    return;
  }

  int bytes;
  if ((bytes = file.write(domDoc.toByteArray(2))) == -1) {
    qDebug() << "Could not write data into the settings file!";
    return;
  }

  file.close();
}

QVariant SettingsModel::data(const QModelIndex& index, int role) const {
  QString roleStr = dataRoleToString(role);
  SettingsItem* item = static_cast<SettingsItem*>(index.internalPointer());
  QDomNode node = item->getNode();
  node = node.firstChild();
  while (!node.isNull()) {
    if (node.isElement() && node.nodeName() == roleStr) {
      if (role == Qt::DecorationRole) {
        return QVariant(item->getIcon());
      }
      QDomElement element = node.toElement();
      QVariant ret =
        variantFromString(element.text(), element.attribute("type"));
      return ret;
    }
    node = node.nextSibling();
  }
  return QVariant();
}

QModelIndex SettingsModel::index(
  int row, int column, const QModelIndex& parent) const {
  if (column != 0) {
    qDebug() << "The model does not allow columns";
    return QModelIndex();
  }

  // Get the parent settings item
  SettingsItem* parentItem;
  if (!parent.isValid()) {
    parentItem = rootItem;
  } else {
    parentItem = static_cast<SettingsItem*>(parent.internalPointer());
  }

  // Find the corresponding row and column for the found settings item
  SettingsItem* childItem = parentItem->getChild(row);
  if (childItem == NULL)
    return QModelIndex();

  return createIndex(row, 0, childItem);
}

QModelIndex SettingsModel::indexFromItem(SettingsItem* item) const {
  if (!item->getParent()) {
    return QModelIndex();
  } else {
    SettingsItem* parent = item->getParent();
    int row = parent->childIndex(item);
    return createIndex(row, 0, item);
  }
}

bool SettingsModel::insertRow(int row, const QModelIndex& parent) {
  SettingsItem* item;
  if (!parent.isValid()) {
    item = rootItem;
  } else {
    item = static_cast<SettingsItem*>(parent.internalPointer());
  }

  int begin = row;
  int end = row;
  if (row > item->rowCount()) {
    begin = item->rowCount();
  }
  beginInsertRows(parent, begin, end);

  // Insertions further than one index after the last row insert more rows
  while (row > item->rowCount()) {
    insertRowInternal(item->rowCount(), item);
  }
  insertRowInternal(row, item);

  endInsertRows();

  return true;
}

SettingsItem* SettingsModel::itemFromIndex(const QModelIndex& index) const {
  if (!index.isValid()) return NULL;

  return static_cast<SettingsItem*>(index.internalPointer());
}

QModelIndex SettingsModel::parent(const QModelIndex& index) const {
  if (!index.isValid()) {
    return index;
  }

  SettingsItem* item = static_cast<SettingsItem*>(index.internalPointer());
  return indexFromItem(item->getParent());
}

int SettingsModel::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid()) {
    return rootItem->rowCount();
  }

  SettingsItem* item = static_cast<SettingsItem*>(parent.internalPointer());
  return item->rowCount();
}

bool SettingsModel::setData(
  const QModelIndex& index, const QVariant& value, int role) {
  // Find the corresponding item element node
  QString roleStr = dataRoleToString(role);
  SettingsItem* item = static_cast<SettingsItem*>(index.internalPointer());
  QDomNode node = item->getNode();

  // Get the data element node
  QDomNodeList nodeList = node.toElement().elementsByTagName(roleStr);
  QDomNode dataNode;
  if (nodeList.isEmpty()) {
    dataNode = domDoc.createElement(roleStr);
    node.appendChild(dataNode);
  } else {
    dataNode = nodeList.item(0);
    // Remove the old content
    QDomNodeList childNodes = dataNode.childNodes();
    for (int i = 0; i < childNodes.count(); ++i) {
      dataNode.removeChild(childNodes.item(i));
    }
  }

  // Set the data and data type into the element
  QDomElement element = dataNode.toElement();
  element.setAttribute("type", value.typeName());
  QString data = variantToString(value);
  element.appendChild(domDoc.createTextNode(data));

  // Special handling needs to be done for decorations
  if (role == Qt::DecorationRole) {
    item->setIcon(QIcon(value.toString()));
  }

  // Emit the dataChanged signal
  emit dataChanged(index, index);
  emit itemChanged(item);
  return true;
}

QModelIndex SettingsModel::sibling(
  int row, int column, const QModelIndex& index) const {
  if (column != 0) {
    qDebug() << "The model does not support columns";
    return QModelIndex();
  }

  if (!index.isValid()) {
    return QModelIndex();
  }

  SettingsItem* item = static_cast<SettingsItem*>(index.internalPointer());
  SettingsItem* parent = item->getParent()->getParent();
  item = parent->getChild(row);
  return createIndex(row, 0, item);
}

QString SettingsModel::dataRoleToString(int role) const {
  switch (role) {
    case Qt::EditRole:
      return "value";
    break;
    case Qt::DecorationRole:
      return "decoration";
    break;
    case Qt::DisplayRole:
      return "description";
    break;
    case Qt::UserRole:
      return "name";
    break;
    case Qt::UserRole+1:
      return "default_value";
    break;
    default:
      return QString("user%1").arg(role-Qt::UserRole);
    break;
  }
}

void SettingsModel::insertRowInternal(int row, SettingsItem* item) {
  SettingsItem* newItem;
  QDomElement element = domDoc.createElement("item");
  newItem = new SettingsItem(element, this);
  if (row == item->rowCount()) {
    item->getNode().appendChild(element);
    item->appendChild(newItem);
  } else {
    QDomNode next = item->getNode().firstChild();
    for (int i = 0; i < row; ++i) {
      next = next.nextSibling();
    }
    item->getNode().insertBefore(element, next);
    item->insertChild(newItem, row);
  }
}

}  // End namespace Core
}  // End namespace Updraft

