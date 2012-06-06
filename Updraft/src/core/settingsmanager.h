#ifndef UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_
#define UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_

#include <QtGui>
#include "ui/settingsdialog.h"
#include "variantfunctions.h"
#include "../settingsgrouptype.h"

namespace Updraft {

class SettingInterface;

namespace Core {

class BasicSetting;
class SettingsModel;
class SettingsItem;

class SettingsManager: public QObject {
  Q_OBJECT

 public:
  /// Prepare the settings manager to a state where settings are loaded
  /// and accessible, but don't do anything gui related.
  /// \note finishInit(); must be called after both SettingsManager and
  /// TranslationManager are created!
  SettingsManager();
  ~SettingsManager();

  /// Finalize the initialization.
  /// Used to break the circular dependency between settings manager
  /// and translation manager.
  void finishInit();

  SettingInterface* addSetting(
    const QString& settingId,
    const QString& description,
    QVariant defaultValue,
    bool hiden = false);

  void addGroup(
    const QString& groupId,
    const QString& description,
    SettingsGroupType type = GROUP_VISIBLE,
    const QString& icon = ":/core/icons/configure.png");

  /// \return Directory where the currently used settings file resides
  QDir getSettingsDir();

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

  /// Searches the standard locations for a settings file and returns
  /// its location.
  QString getSettingsFilename();

  /// Check if file settings.xml exists in dir/dir2.
  /// \return Absolute path to settings.xml, or empty string if it wasn't found.
  QString checkSettingsXml(const QString &dir1, const QString &dir2);

  /// Create file settings.xml in dir/dir2.
  /// \return Absolute path to settings.xml, or empty string if
  /// something went wrong.
  QString createSettingsXml(const QString &dir1, const QString &dir2);

  QString settingsFile;

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

