#ifndef UPDRAFT_SRC_COREINTERFACE_H_
#define UPDRAFT_SRC_COREINTERFACE_H_

#include <QString>
#include <QIcon>

#include "menuinterface.h"
#include "tabinterface.h"
#include "fileregistration.h"
#include "maplayergroupinterface.h"
#include "settinginterface.h"

class QWidget;
class QMainWindow;

namespace Updraft {

/// Exposes core functionalities to plugins.
/// A call to methods of this interface automagically contains
/// a pointer to calling plugin (this is ensured in core/coreimplementation.cpp)
class CoreInterface {
 public:
  virtual ~CoreInterface() {}

  /// Returns main window pointer. Used for example for modal dialogs
  virtual QMainWindow* getMainWindow() = 0;

  /// Create an entry in the main menu.
  virtual MenuInterface* createMenu(QString title) = 0;

  /// Returns pointer to the instance of a system menu
  /// \param menu which system menu instance to return
  virtual MenuInterface* getSystemMenu(SystemMenu menu) = 0;

  /// Creates map layer group and adds it to the tree view in the left pane.
  /// To remove MapLayerGroup use C++ delete.
  /// \param title label for the item in the tree view
  /// \return Pointer to the new MapLayerGroup
  virtual MapLayerGroupInterface* createMapLayerGroup(const QString &title) = 0;

  /// Open a Tab with any widget.
  /// Takes ownership of content.
  virtual TabInterface* createTab(QWidget* content, QString title) = 0;

  /// Removes tab from the bottom pane
  /// The tab class given to this function is deleted!
  /// Equivalent to tab->close()
  virtual void removeTab(TabInterface* tab) = 0;

  /// This plugin knows how to open the file!
  /// \return Identifier of the newly registered filetype
  virtual void registerFiletype(const FileRegistration &registration) = 0;

  /// Gets path of the application data directory.
  /// \return Full path to the application directory
  virtual QString getDataDirectory() = 0;

  /// Adds a group into the settings dialog.
  /// If a setting group with the given ID already exists, its description
  /// and icon are changed to the new ones.
  /// \param groupId An identifier of the group to create or change description.
  /// \param description Description of the group. This is displayed in the
  ///                    dialog next to the settings icon.
  /// \param icon The icon of the settings group. Default is a wrench icon.
  virtual void addSettingsGroup(
    const QString& groupId,
    const QString& description,
    const QString& icon = ":/core/icons/configure.png") = 0;

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
  /// \param hidden Whether the setting should be hidden or displayed in the
  ///        settings dialog.
  /// \return Interface for setting and reading the setting value. In case of
  ///         failure, NULL is returned.
  virtual SettingInterface* addSetting(
    const QString& settingId,
    const QString& description,
    QVariant initValue,
    bool hidden = false) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_COREINTERFACE_H_

