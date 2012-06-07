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

/// The main settings class. Owns the SettingsModel instance, executes the
/// settings dialog and allows for setting group and setting creation.
class SettingsManager: public QObject {
  Q_OBJECT

 public:
  /// Prepare the settings manager to a state where settings are loaded
  /// and accessible, but don't do anything gui-related.
  /// \note finishInit(); must be called after both SettingsManager and
  /// TranslationManager are created!
  SettingsManager();
  ~SettingsManager();

  /// Finalize the initialization.
  /// Used to break the circular dependency between settings manager
  /// and translation manager.
  void finishInit();

  /// Adds a setting into the settings dialog.
  /// If the setting already exists, its value is not set to the provided value
  /// but instead, the value is read from the existing setting. Ownership of
  /// the setting interface is transfered to the caller of this function.
  /// \param settingId Identifier for the setting, prepended with a ':' and the
  ///        settings group identifier, where this setting will reside. If the
  ///        group with the given identifier does not exist, a new one will be
  ///        created with a default icon and name identical to the identifier.
  /// \param description Description of the setting. This will be displayed in
  ///        the settings dialog.
  /// \param initValue This is the initial value for the setting if it is newly
  ///        created.
  /// \param type Whether the setting is being added to a visible, advanced or
  ///        hidden group.
  /// \return Interface for setting and reading the setting value. In case of
  ///         failure, NULL is returned.
  SettingInterface* addSetting(
    const QString& settingId,
    const QString& description,
    QVariant defaultValue,
    SettingsGroupType type = GROUP_VISIBLE);

  /// Adds a group into the settings dialog.
  /// If a setting group with the given ID already exists, its description
  /// and icon are changed to the new ones.
  /// \param groupId An identifier of the group to create or change description.
  /// \param description Description of the group. This is displayed in the
  ///                    dialog next to the settings icon.
  /// \param type Whether the group should be visible, advanced or hidden.
  /// \param icon The icon of the settings group. Default is a wrench icon.
  void addGroup(
    const QString& groupId,
    const QString& description,
    SettingsGroupType type = GROUP_VISIBLE,
    const QString& icon = ":/core/icons/configure.png");

  /// \return Directory where the currently used settings file resides
  QDir getSettingsDir();

  friend class BasicSetting;

 public slots:
  /// Executes the settings dialog
  void execDialog();

  /// Resets all the defined settings to their default values.
  void resetToDefaults();

 private slots:
  /// Handles the value change of a settings item by calling the valueChanged()
  /// signals of all the BasicSetting s pointing to it.
  void itemValueChanged(SettingsItem* item);

 private:
  /// Returns an index for the given settings group.
  /// \param groupId The string identifier of the gropu whose index should be
  ///        retrieved. The identifier should contain the suffixes that denote
  ///        the type of the group.
  /// \return The model index of the desired settings group.
  QModelIndex getGroup(const QString& groupId);

  /// Returns an index for the given setting.
  /// \param settingId The identifier of the setting without the group name.
  /// \param groupIndex The model index of the group for the setting.
  /// \return The model index for the desired setting.
  QModelIndex getSetting(const QString& settingId, QModelIndex groupIndex);

  /// Internal helper function for adding groups. Does not know anything about
  /// the group types (they are encoded in the group suffix already). If the
  /// desired group already exists, its description and icon are overwritten.
  /// \param groupId The string identifier of the group (with the suffix
  ///        denoting the group type.
  /// \param description The description of the group that will be shown in the
  ///        settings dialog.
  /// \param icon The decorating icon for this group for the settings dialog.
  /// \return Model index of the desired group.
  QModelIndex addGroupInternal(
    const QString& groupId,
    const QString& description,
    const QString& icon);

  /// Registers a BasicSetting instance to the settings item.
  /// This is to be able to call all BasicSetting instances on the setting's
  /// value change.
  /// \param item The SettingsItem representing the desired setting.
  /// \param setting The new BasicSetting instance that should be registered.
  void registerSetting(SettingsItem* item, BasicSetting* setting);

  /// Unregisters a BasicSetting instance that was registred using the
  /// registerSetting() method. This is usually done when the BasicSetting
  /// instance is destructed.
  /// \param item The setting item, to which the BasicSetting instance was
  ///             registered.
  /// \param setting The BasicSetting instance to be unregistered.
  void unregisterSetting(SettingsItem* item, BasicSetting* setting);

  /// Searches the standard locations for a settings file.
  /// \return The location of the settings file.
  QString getSettingsFilename();

  /// Check if file settings.xml exists in dir/dir2.
  /// \param dir1 Path to the directory containing the settings file.
  /// \param dir2 Name of the directory containing the settings file.
  /// \return Absolute path to settings.xml, or empty string if it wasn't found.
  QString checkSettingsXml(const QString &dir1, const QString &dir2);

  /// Create file settings.xml in dir/dir2.
  /// \param dir1 Path to the directory that should contain the settings file.
  /// \param dir2 Name of the directory that should contain the settings file.
  /// \return Absolute path to settings.xml, or empty string if something went
  ///         wrong.
  QString createSettingsXml(const QString &dir1, const QString &dir2);

  /// Filename of the settings file.
  QString settingsFile;

  /// The action that executes the settings dialog.
  QAction* settingsAction;

  /// The settings dialog.
  SettingsDialog* dialog;

  /// The settings data model.
  SettingsModel* model;

  /// Regular expression that matches a setting or group identifier
  QRegExp idRegExp;

  /// The associative container of SettingsItem instances to BasicSetting
  /// instances. Used in methods registerSetting() and unregisterSetting().
  QMultiHash<SettingsItem*, BasicSetting*> settings;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSMANAGER_H_

