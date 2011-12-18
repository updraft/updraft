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

Menu* CoreImplementation::getSystemMenu(SystemMenu menu) {
  MainWindow* win = parent->getMainWindow();
  return win->getSystemMenu(menu);
}

TabInterface* CoreImplementation::createTab(QWidget* content, QString title) {
    return parent->getMainWindow()->createTab(content, title);
}

void CoreImplementation::removeTab(TabInterface* tab) {
  tab->close();
}

}  // End namespace Core
}  // End namespace Updraft

