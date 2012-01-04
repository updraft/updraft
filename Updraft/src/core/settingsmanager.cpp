#include "settingsmanager.h"

#include "updraft.h"
#include "ui/mainwindow.h"
#include "menu.h"
#include "../menuinterface.h"
#include "basicsetting.h"
#include "ui_settingsdialog.h"

namespace Updraft {
namespace Core {

SettingsManager::SettingsManager(): dialog(new SettingsDialog(NULL)) {
  // Initialize id regexp for identifier pattern matching
  idRegExp = QRegExp("[a-zA-Z0-9_]+");

  model = new QStandardItemModel(this);

  // Set the dialog's model
  dialog->setModel(model);

  // Create an action that shows the dialog and add it to the menu
  MainWindow* win = updraft->mainWindow;
  Menu* toolsMenu = win->getSystemMenu(MENU_TOOLS);

  settingsAction = new QAction(QIcon(":/core/icons/configure.png"), tr("Settings..."), this); // NOLINT
  settingsAction->setIconVisibleInMenu(true);
  QObject::connect(settingsAction, SIGNAL(triggered()), dialog, SLOT(exec()));
  toolsMenu->insertAction(100, settingsAction);
}

SettingsManager::~SettingsManager() {
  // TODO(cestmir): We probably need to destroy this, since it has no parent
  delete dialog;
}

SettingInterface* SettingsManager::addSetting(
  const QString& settingId,
  const QString& description,
  QVariant initValue) {

  QStringList identifiers = settingId.split(':');
  if (identifiers.size() != 2) {
    qDebug() <<
    "There must be exactly two parts in the setting ID, separated by ':'";
    return NULL;
  }

  QString groupIdPart = identifiers[0];
  QString settingIdPart = identifiers[1];

  if (!idRegExp.exactMatch(groupIdPart)) {
    qDebug() << "The group ID part in setting ID has incorrect syntax";
    qDebug() << "It was: " << groupIdPart;
    return NULL;
  }

  if (!idRegExp.exactMatch(settingIdPart)) {
    qDebug() << "The setting ID part in setting ID has incorrect syntax";
    return NULL;
  }

  QModelIndex groupIndex = getGroup(groupIdPart);
  if (!groupIndex.isValid()) {
    groupIndex = addGroup(
      groupIdPart,
      groupIdPart,
      QIcon(":/core/icons/configure.png"));
  }

  // Try to find the setting
  QModelIndex settingIndex = getSetting(settingIdPart, groupIndex);

  QStandardItem* valueItem;
  if (!settingIndex.isValid()) {
    QStandardItem* idItem = new QStandardItem(settingIdPart);
    QStandardItem* descItem = new QStandardItem(description);
    valueItem = new QStandardItem();
    valueItem->setData(initValue, Qt::EditRole);

    QStandardItem* groupItem = model->itemFromIndex(groupIndex);
    int groupRows = groupItem->rowCount();
    groupItem->setChild(groupRows, 0, idItem);
    groupItem->setChild(groupRows, 1, descItem);
    groupItem->setChild(groupRows, 2, valueItem);
  } else {
    QModelIndex valueIndex = model->sibling(settingIndex.row(), 2, settingIndex);
    valueItem = model->itemFromIndex(valueIndex);
  }

  return new BasicSetting(valueItem);
}

QModelIndex SettingsManager::addGroup(
  const QString& groupId,
  const QString& description,
  QIcon icon) {
  if (!model) {
    qDebug() << "Model was not created before creating a settings group!";
    return QModelIndex();
  }

  // First find if the group does not exist already
  QModelIndex groupIndex = getGroup(groupId);

  // Get the items from the group row and change them.
  // If the group was not found, create it.
  QStandardItem* idItem;
  QStandardItem* descItem;
  if (!groupIndex.isValid()) {
    idItem = new QStandardItem(groupId);
    descItem = new QStandardItem(icon, description);

    int rows = model->rowCount();
    model->setItem(rows, 1, idItem);
    model->setItem(rows, 0, descItem);
    groupIndex = model->indexFromItem(idItem);
  } else {
    QModelIndex descIndex = model->sibling(groupIndex.row(), 0, groupIndex);
    model->setData(descIndex, description, Qt::DisplayRole);
    model->setData(descIndex, icon, Qt::DecorationRole);
  }

  int newColumnWidth = dialog->ui->topView->sizeHintForColumn(0);
  QSize maxSize = dialog->ui->topView->maximumSize();
  maxSize.setWidth(newColumnWidth + 10);   // Add some extra space
  dialog->ui->topView->setMaximumSize(maxSize);

  return groupIndex;
}

QModelIndex SettingsManager::getGroup(const QString& groupId) {
  QModelIndex groupIndex;

  int i;
  for (i = 0; i < model->rowCount(); i++) {
    groupIndex = model->index(i, 1);
    QVariant modelGroupId = groupIndex.data();
    if (modelGroupId.toString() == groupId)
      break;
  }
  if (i == model->rowCount()) {
    groupIndex = QModelIndex();
  }

  return groupIndex;
}

QModelIndex SettingsManager::getSetting(const QString& settingId, QModelIndex groupIndex) {
  QModelIndex settingIndex;

  int i;
  for (i = 0; i < model->rowCount(groupIndex); i++) {
    settingIndex = model->index(i, 0, groupIndex);
    QVariant modelSettingId = settingIndex.data();
    if (modelSettingId.toString() == settingId)
      break;
  }
  if (i == model->rowCount(groupIndex)) {
    settingIndex = QModelIndex();
  }

  return settingIndex;
}

}  // End namespace Core
}  // End namespace Updraft

