#include "coreimplementation.h"

#include "updraft.h"

namespace Updraft {
namespace Core {

CoreImplementation::CoreImplementation(PluginBase *pl)
    : plugin(pl) {}

QMainWindow* CoreImplementation::getMainWindow() {
  return static_cast<QMainWindow*>(updraft->mainWindow);
}

MenuInterface* CoreImplementation::createMenu(QString title) {
  MainWindow* win = updraft->mainWindow;

  return win->createMenu(title);
}

MenuInterface* CoreImplementation::getSystemMenu(SystemMenu menu) {
  MainWindow* win = updraft->mainWindow;
  return win->getSystemMenu(menu);
}

const osg::EllipsoidModel* CoreImplementation::getEllipsoidModel() {
  return updraft->sceneManager->getMapNode()->getMap()->getProfile()->
    getSRS()->getEllipsoid();
}

MapLayerGroupInterface* CoreImplementation::createMapLayerGroup
  (const QString &title) {
  osg::Group* group = updraft->sceneManager->newGroup();
  osgEarth::MapNode* map = updraft->sceneManager->getMapNode();
  return updraft->mainWindow->createMapLayerGroup(title, group, map);
}

TabInterface* CoreImplementation::createTab(QWidget* content, QString title) {
    return updraft->mainWindow->createTab(content, title);
}

void CoreImplementation::removeTab(TabInterface* tab) {
  tab->close();
}

void CoreImplementation::registerFiletype(
  const FileRegistration &registration) {
  FileTypeManager *manager = updraft->fileTypeManager;
  manager->registerFiletype(registration);
}

QString CoreImplementation::getDataDirectory() {
  // TODO(Tom): Load data directory from settings.
  return updraft->getDataDirectory();
}

void CoreImplementation::addSettingsGroup(
    const QString& groupId,
    const QString& description,
    const QString& icon) {
  updraft->settingsManager->addGroup(groupId, description, icon);
}

SettingInterface* CoreImplementation::addSetting(
    const QString& settingId,
    const QString& description,
    QVariant initVal,
    bool hidden) {
  return updraft->settingsManager->
    addSetting(settingId, description, initVal, hidden);
}

osg::Group* CoreImplementation::getSimpleGroup() {
  return updraft->sceneManager->getSimpleGroup();
}

void CoreImplementation::registerOsgNode(osg::Node* node, MapObject* mapObject) {
  updraft->sceneManager->registerOsgNode(node, mapObject);
}

}  // End namespace Core
}  // End namespace Updraft

