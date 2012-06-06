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

SettingsManager::SettingsManager()
  : dialog(NULL) {
  // Initialize id regexp for identifier pattern matching
  idRegExp = QRegExp("[a-zA-Z0-9_]+");

  settingsFile = getSettingsFilename();

  model = new SettingsModel();
  model->loadSettings(settingsFile);

  // Let the manager know whenever the model changes
  connect(model, SIGNAL(itemChanged(SettingsItem*)),
    this, SLOT(itemValueChanged(SettingsItem*)));
}

SettingsManager::~SettingsManager() {
  model->saveSettings(settingsFile);
  delete model;
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
    model->setData(settingIndex, defaultValue, ValueRole);
    model->setData(settingIndex, defaultValue, DefaultValueRole);
    model->setData(settingIndex, settingIdPart, NameRole);
    model->setData(settingIndex, description, DescriptionRole);
  }

  SettingsItem* settingItem = model->itemFromIndex(settingIndex);
  BasicSetting* setting = new BasicSetting(settingItem, this);
  registerSetting(settingItem, setting);
  return setting;
}

void SettingsManager::addGroup(
  const QString& groupId,
  const QString& description,
  SettingsGroupType type,
  const QString& icon) {
  switch (type) {
    case GROUP_VISIBLE:
      addGroupInternal(groupId + "_visible", description, icon);
    break;
    case GROUP_HIDDEN:
      addGroupInternal(groupId + "_hidden", description, icon);
    break;
    case GROUP_ADVANCED:
      addGroupInternal(groupId + "_advanced", description, icon);
    break;
  }
}

QDir SettingsManager::getSettingsDir() {
  QFileInfo info(settingsFile);
  return info.absoluteDir();
}

void SettingsManager::execDialog() {
  dialog = new SettingsDialog(updraft->mainWindow, this);
  dialog->setModel(model);

  if (dialog->exec() == QDialog::Accepted) {
    dialog->commitEditorData();
  } else {
    dialog->resetEditors();
  }

  delete dialog;
  dialog = NULL;
}

void SettingsManager::resetToDefaults() {
  for (int i = 0; i < model->rowCount(); i++) {
    QModelIndex groupIndex = model->index(i, 0);
    for (int j = 0; j < model->rowCount(groupIndex); ++j) {
      QModelIndex itemIndex = model->index(j, 0, groupIndex);
      QVariant defaultData = model->data(itemIndex, DefaultValueRole);
      QVariant currentData = model->data(itemIndex, ValueRole);
      // Only set the default data if it differs from the current data
      if (!variantsEqual(defaultData, currentData)) {
        model->setData(itemIndex, defaultData, ValueRole);
      }
    }
  }

  if (dialog) {
    dialog->resetEditors();
  }
}

void SettingsManager::itemValueChanged(SettingsItem* item) {
  QHash<SettingsItem*, BasicSetting*>::const_iterator it =
    settings.find(item);
  while (it != settings.end() && it.key() == item) {
    it.value()->emitValueChanged();
    ++it;
  }
}

QModelIndex SettingsManager::getGroup(const QString& groupId) {
  QModelIndex groupIndex;

  int i;
  for (i = 0; i < model->rowCount(); i++) {
    groupIndex = model->index(i, 0);
    QVariant modelGroupId = model->data(groupIndex, NameRole);
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
    QVariant modelSettingId =
      model->data(settingIndex, NameRole);
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
    model->setData(groupIndex, description, DescriptionRole);
    model->setData(groupIndex, groupId, NameRole);
    model->setData(groupIndex, icon, DecorationRole);
  } else {
    model->setData(groupIndex, description, DescriptionRole);
    model->setData(groupIndex, icon, DecorationRole);
  }

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
  QString homeDir =
    QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
  QString appDataDir =
    QDesktopServices::storageLocation(QDesktopServices::DataLocation);
  QString executableDir = updraft->applicationDirPath();

  QString settingsFilePath;

#ifdef Q_WS_X11
  if (settingsFilePath.isEmpty()) {
    settingsFilePath = checkSettingsXml(homeDir, ".updraft");
  }
#endif

  if (settingsFilePath.isEmpty()) {
    settingsFilePath = checkSettingsXml(appDataDir, "updraft");
  }

  if (settingsFilePath.isEmpty()) {
    settingsFilePath = checkSettingsXml(executableDir, QString());
  }

  if (settingsFilePath.isEmpty()) {
#if defined(Q_WS_X11)
    settingsFilePath = createSettingsXml(homeDir, ".updraft");
#elif defined(Q_WS_WIN)
    settingsFilePath = createSettingsXml(executableDir, QString());
#else
    settingsFilePath = createSettingsXml(appDataDir, "updraft");
#endif
  }

  return settingsFilePath;
}

QString SettingsManager::checkSettingsXml(
  const QString &dir1, const QString& dir2) {
  QDir dir = QDir(dir1);

  qDebug() << "Looking for settings.xml in " << dir.absoluteFilePath(dir2);


  if (!dir2.isEmpty()) {
    if (!dir.cd(dir2)) {
      return QString();
    }
  }

  QFileInfo file(dir, "settings.xml");

  if (
    file.exists() &&
    file.isFile() &&
    file.isReadable() &&
    file.isWritable()) {
    return file.absoluteFilePath();
  }

  return QString();
}

QString SettingsManager::createSettingsXml(
  const QString &dir1, const QString &dir2) {
  QDir dir = QDir(dir1);

  qDebug() << "Creating settings.xml in " << dir.absoluteFilePath(dir2);

  // Change to the updraft settings directory
  if (!dir.exists(dir2)) {
    dir.mkdir(dir2);

    if (!dir.cd(dir2)) {
      qDebug() << "Failed to create directory for settings file.";
      dir.rmdir(dir2);
      return QString();
    }
  } else {
    if (!dir.cd(dir2)) {
      qDebug() << "Failed to create directory for settings file.";
      return QString();
    }
  }

  QString path = dir.absoluteFilePath("settings.xml");
  QFile file(path);

  if (!file.open(QFile::WriteOnly)) {
    qDebug() << "Failed to create settings.xml file.";
    return QString();
  } else {
    file.close();
    return path;
  }
}

}  // End namespace Core
}  // End namespace Updraft

