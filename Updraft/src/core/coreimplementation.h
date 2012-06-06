#ifndef UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_
#define UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_

#include "../coreinterface.h"

namespace Updraft {

namespace Util {
  class Ellipsoid;
}

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

  const Util::Ellipsoid* getEllipsoid() const;

  MapLayerGroupInterface* createMapLayerGroup(const QString &title);

  TabInterface* createTab(QWidget* content, QString title);
  void removeTab(TabInterface* tab);

  void registerFiletype(const FileRegistration &registration);

  QDir getDataDirectory();
  QDir getResourcesDirectory();

  void addSettingsGroup(
    const QString& groupId,
    const QString& description,
    SettingsGroupType type,
    const QString& icon);

  SettingInterface* addSetting(
    const QString& settingId,
    const QString& description,
    QVariant initValue,
    bool hidden);

  osg::Group* getSimpleGroup();

  void registerOsgNode(osg::Node* node, MapObject* mapObject);

  osgEarth::Util::ElevationManager* getElevationManager();

  const osg::EllipsoidModel* getCurrentMapEllipsoid();

 private:
  PluginBase* plugin;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_

