#include "coreimplementation.h"

#include "ui/mainwindow.h"

namespace Updraft {
namespace Core {

CoreImplementation::CoreImplementation(UpdraftParent* par, PluginBase *pl)
    : plugin(pl), parent(par) {}

QMainWindow* CoreImplementation::getMainWindow() {
  return static_cast<QMainWindow*>(parent->getMainWindow());
}

Menu* CoreImplementation::createMenu(QString title) {
  MainWindow* win = parent->getMainWindow();

  return win->createMenu(title);
}

void CoreImplementation::removeMenu(Menu* menu) {
  MainWindow* win = parent->getMainWindow();

  win->removeMenu(menu);
}

Menu* CoreImplementation::getSystemMenu(SystemMenu menu) {
  MainWindow* win = parent->getMainWindow();
  return win->getSystemMenu(menu);
}

Tab* CoreImplementation::createTab(QWidget* content, QString title) {
    return parent->getMainWindow()->createTab(content, title, plugin);
}

void CoreImplementation::removeTab(Tab* tab) {
  tab->close();
}

}  // End namespace Core
}  // End namespace Updraft

