#ifndef UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_
#define UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_

#include <QtGui>
#include "ui/settingsdialog.h"
#include "variantfunctions.h"

namespace Updraft {

class SettingInterface;

namespace Core {

class BasicSetting;
class SettingsModel;
class SettingsItem;

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

  void addGroup(
    const QString& groupId,
    const QString& description,
    const QString& icon);

  friend class BasicSetting;

 public slots:
  void execDialog();
  void resetToDefaults();

 private slots:
  void itemValueChanged(SettingsItem* item);

 private:
  /// Returns an index for the given settings group.
  QModelIndex getGroup(const QString& groupId);
  QModelIndex getSetting(const QString& settingId, QModelIndex groupIndex);

  QModelIndex addGroupInternal(
    const QString& groupId,
    const QString& description,
    const QString& icon);

  void registerSetting(SettingsItem* item, BasicSetting* setting);
  void unregisterSetting(SettingsItem* item, BasicSetting* setting);

  QAction* settingsAction;

  SettingsDialog* dialog;

  // TODO(cestmir): This needs to be converted to our own editable model
  // So far, this is here only to test the specialized settings view
  SettingsModel* model;

  QRegExp idRegExp;

  QMultiHash<SettingsItem*, BasicSetting*> settings;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_

