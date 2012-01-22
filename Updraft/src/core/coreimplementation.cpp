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

MapLayerGroup* CoreImplementation::createMapLayerGroup(const QString &title) {
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

}  // End namespace Core
}  // End namespace Updraft

