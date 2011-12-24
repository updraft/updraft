#include "coreimplementation.h"

#include "ui/mainwindow.h"
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
  return updraft->mainWindow->createMapLayerGroup(title);
}

TabInterface* CoreImplementation::createTab(QWidget* content, QString title) {
    return updraft->mainWindow->createTab(content, title);
}

void CoreImplementation::removeTab(TabInterface* tab) {
  tab->close();
}

/// This plugin knows how to open the file!
/// \return Identifier of the newly registered filetype
void CoreImplementation::registerFiletype(QString extension,
  QString description, FileCategory category) {
  FileTypeManager *manager = updraft->fileTypeManager;
  manager->registerFiletype(extension, description, category, plugin);
}

}  // End namespace Core
}  // End namespace Updraft

