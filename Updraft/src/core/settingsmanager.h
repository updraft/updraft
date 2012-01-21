#ifndef UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_
#define UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_

#include <QtGui>
#include "ui/settingsdialog.h"

namespace Updraft {

class SettingInterface;

namespace Core {

class BasicSetting;

class SettingsManager: public QObject {
Q_OBJECT

 public:
  SettingsManager();
  ~SettingsManager();

  SettingInterface* addSetting(
    const QString& settingId,
    const QString& description,
    QVariant defaultValue,
    bool hiden = false);

  QModelIndex addGroup(
    const QString& groupId,
    const QString& description,
    QIcon icon);

  friend class BasicSetting;

 public slots:
  void execDialog();
  void resetToDefaults();

 private slots:
  void itemValueChanged(QStandardItem* item);

 private:
  /// Returns an index for the given settings group.
  QModelIndex getGroup(const QString& groupId);
  QModelIndex getSetting(const QString& settingId, QModelIndex groupIndex);

  QModelIndex addGroupInternal(
    const QString& groupId,
    const QString& description,
    QIcon icon);

  void registerSetting(QStandardItem* item, BasicSetting* setting);
  void unregisterSetting(QStandardItem* item, BasicSetting* setting);

  QAction* settingsAction;

  SettingsDialog* dialog;

  // TODO(cestmir): This needs to be converted to our own editable model
  // So far, this is here only to test the specialized settings view
  QStandardItemModel* model;

  QRegExp idRegExp;

  QMultiHash<QStandardItem*, BasicSetting*> settings;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_

