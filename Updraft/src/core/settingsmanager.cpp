#include "settingsmanager.h"

#include <QDesktopServices>

#include "updraft.h"
#include "ui/mainwindow.h"
#include "ui/menu.h"
#include "../menuinterface.h"
#include "basicsetting.h"
#include "ui_settingsdialog.h"
#include "settingsmodel.h"

namespace Updraft {
namespace Core {

SettingsManager::SettingsManager(): dialog(new SettingsDialog(NULL, this)) {
  // Initialize id regexp for identifier pattern matching
  idRegExp = QRegExp("[a-zA-Z0-9_]+");

  model = new SettingsModel();
  model->loadSettings(getSettingsFilename());

  // Set the dialog's model
  dialog->setModel(model);

  // Let the manager know whenever the model changes
  connect(model, SIGNAL(itemChanged(SettingsItem*)),
    this, SLOT(itemValueChanged(SettingsItem*)));

  // Create an action that shows the dialog and add it to the menu
  MainWindow* win = updraft->mainWindow;
  Menu* toolsMenu = win->getSystemMenu(MENU_TOOLS);

  settingsAction = new QAction(QIcon(":/core/icons/configure.png"),
    tr("Settings..."), this);
  settingsAction->setIconVisibleInMenu(true);
  connect(settingsAction, SIGNAL(triggered()),
    this, SLOT(execDialog()));
  toolsMenu->insertAction(100, settingsAction);
}

SettingsManager::~SettingsManager() {
  model->saveSettings(getSettingsFilename());
  delete model;

  // TODO(cestmir): We probably need to destroy this, since it has no parent
  delete dialog;
}

SettingInterface* SettingsManager::addSetting(
  const QString& settingId,
  const QString& description,
  QVariant defaultValue,
  bool hidden) {
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

  // Do we want group for hidden settings or visible ones?
  if (hidden) {
    groupIdPart = groupIdPart + "_hidden";
  } else {
    groupIdPart = groupIdPart + "_visible";
  }

  QModelIndex groupIndex = getGroup(groupIdPart);
  if (!groupIndex.isValid()) {
    groupIndex = addGroupInternal(
      groupIdPart,
      identifiers[0],
      ":/core/icons/configure.png");
  }

  // Try to find the setting
  QModelIndex settingIndex = getSetting(settingIdPart, groupIndex);

  if (!settingIndex.isValid()) {
    // Insert the new settings item
    int groupRows = model->rowCount(groupIndex);
    model->insertRow(groupRows, groupIndex);
    settingIndex = model->index(groupRows, 0, groupIndex);

    // Set the data
    model->setData(settingIndex, defaultValue, Qt::EditRole);
    model->setData(settingIndex, defaultValue, Qt::UserRole+1);
    model->setData(settingIndex, settingIdPart, Qt::UserRole);
    model->setData(settingIndex, description, Qt::DisplayRole);
  }

  SettingsItem* settingItem = model->itemFromIndex(settingIndex);
  BasicSetting* setting = new BasicSetting(settingItem, this);
  registerSetting(settingItem, setting);
  return setting;
}

void SettingsManager::addGroup(
  const QString& groupId,
  const QString& description,
  const QString& icon) {
  addGroupInternal(groupId + "_visible", description, icon);
  addGroupInternal(groupId + "_hidden", description + tr(" (hidden)"), icon);
}

void SettingsManager::execDialog() {
  if (dialog->exec() == QDialog::Accepted) {
    dialog->commitEditorData();
  } else {
    dialog->resetEditors();
  }
}

void SettingsManager::resetToDefaults() {
  for (int i = 0; i < model->rowCount(); i++) {
    QModelIndex groupIndex = model->index(i, 0);
    for (int j = 0; j < model->rowCount(groupIndex); ++j) {
      QModelIndex itemIndex = model->index(j, 0, groupIndex);
      QVariant defaultData = model->data(itemIndex, Qt::UserRole+1);
      QVariant currentData = model->data(itemIndex, Qt::EditRole);
      // Only set the default data if it differs from the current data
      if (!variantsEqual(defaultData, currentData)) {
        model->setData(itemIndex, defaultData, Qt::EditRole);
      }
    }
  }

  dialog->resetEditors();
}

void SettingsManager::itemValueChanged(SettingsItem* item) {
  QHash<SettingsItem*, BasicSetting*>::const_iterator it =
    settings.find(item);
  while (it != settings.end()) {
    it.value()->emitValueChanged();
    ++it;
  }
}

QModelIndex SettingsManager::getGroup(const QString& groupId) {
  QModelIndex groupIndex;

  int i;
  for (i = 0; i < model->rowCount(); i++) {
    groupIndex = model->index(i, 0);
    QVariant modelGroupId = model->data(groupIndex, Qt::UserRole);
    if (modelGroupId.toString() == groupId) {
      break;
    }
  }
  if (i == model->rowCount()) {
    groupIndex = QModelIndex();
  }

  return groupIndex;
}

QModelIndex SettingsManager::getSetting(
  const QString& settingId,
  QModelIndex groupIndex) {
  QModelIndex settingIndex;

  int i;
  for (i = 0; i < model->rowCount(groupIndex); i++) {
    settingIndex = model->index(i, 0, groupIndex);
    QVariant modelSettingId = model->data(settingIndex, Qt::UserRole);
    if (modelSettingId.toString() == settingId)
      break;
  }
  if (i == model->rowCount(groupIndex)) {
    settingIndex = QModelIndex();
  }

  return settingIndex;
}

QModelIndex SettingsManager::addGroupInternal(
  const QString& groupId,
  const QString& description,
  const QString& icon) {
  if (!model) {
    qDebug() << "Model was not created before creating a settings group!";
    return QModelIndex();
  }

  // First find if the group does not exist already
  QModelIndex groupIndex = getGroup(groupId);

  // Get the items from the group row and change them.
  // If the group was not found, create it.
  if (!groupIndex.isValid()) {
    int rows = model->rowCount();
    model->insertRow(rows);

    groupIndex = model->index(rows, 0);
    model->setData(groupIndex, description, Qt::DisplayRole);
    model->setData(groupIndex, groupId, Qt::UserRole);
    model->setData(groupIndex, icon, Qt::DecorationRole);
  } else {
    model->setData(groupIndex, description, Qt::DisplayRole);
    model->setData(groupIndex, icon, Qt::DecorationRole);
  }

  dialog->recalculateTopViewWidth();

  return groupIndex;
}

void SettingsManager::registerSetting(
  SettingsItem* item,
  BasicSetting* setting) {
  settings.insert(item, setting);
}

void SettingsManager::unregisterSetting(
  SettingsItem* item,
  BasicSetting* setting) {
  settings.remove(item, setting);
}

QString SettingsManager::getSettingsFilename() {
  QString homePath =
    QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
  QString appDataPath =
    QDesktopServices::storageLocation(QDesktopServices::DataLocation);

  QString settingsFile;

  if (trySettingsPath(&settingsFile, homePath)) return settingsFile;
  if (trySettingsPath(&settingsFile, appDataPath)) return settingsFile;
  if (trySettingsPath(&settingsFile, QDir::currentPath())) return settingsFile;

  qDebug() << "None of the default setting directories worked";
  return "";
}

bool SettingsManager::trySettingsPath(QString* settingsFile, QString path) {
  QDir settingsDir(path);
  qDebug() << "Searching for settings in " << settingsDir.filePath("updraft");

  if (!settingsDir.exists("updraft")) {
    settingsDir.mkdir("updraft");
  }

  if (settingsDir.cd("updraft")) {
    *settingsFile = settingsDir.filePath("settings.xml");
    return true;
  }

  return false;
}

}  // End namespace Core
}  // End namespace Updraft

