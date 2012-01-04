#ifndef UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_
#define UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_

#include "../coreinterface.h"

namespace Updraft {

class PluginBase;

namespace Core {

/// Implements the core functionalities avaliable to plugins.
/// There is an instance of this class for every loaded plugin.
class CoreImplementation : public CoreInterface {
 public:
  explicit CoreImplementation(PluginBase *pl);

  QMainWindow* getMainWindow();

  MenuInterface* createMenu(QString title);
  MenuInterface* getSystemMenu(SystemMenu menu);

  virtual MapLayerGroup* createMapLayerGroup(const QString &title);

  TabInterface* createTab(QWidget* content, QString title);
  void removeTab(TabInterface* tab);

  /// This plugin knows how to open the file!
  /// \return Identifier of the newly registered filetype
  void registerFiletype(QString extension, QString description,
    FileCategory category);

  void addSettingsGroup(
    const QString& groupId,
    const QString& description,
    QIcon icon);

  SettingInterface* addSetting(
    const QString& settingId,
    const QString& description,
    QVariant initValue);

 private:
  PluginBase* plugin;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_

