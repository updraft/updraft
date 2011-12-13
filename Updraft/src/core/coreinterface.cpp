#include "../coreinterface.h"
#include "updraftparent.h"
#include "ui/mainwindow.h"

namespace Updraft {

using Core::MainWindow;

QMainWindow* CoreInterface::getMainWindow() {
  return static_cast<QMainWindow*>(parent->getMainWindow());
}

Core::Menu* CoreInterface::createMenu(QString title) {
  MainWindow* win = parent->getMainWindow();

  return win->createMenu(title);
}

void CoreInterface::removeMenu(Core::Menu* menu) {
  MainWindow* win = parent->getMainWindow();

  win->removeMenu(menu);
}

Core::Menu* CoreInterface::getSystemMenu(SystemMenu menu) {
  Core::MainWindow* win = parent->getMainWindow();
  return win->getSystemMenu(menu);
}

/// Create a new tab in the bottom pane.
/// Takes ownership of content.
Core::Tab* CoreInterface::createTab(QWidget* content, QString title) {
    return parent->getMainWindow()->createTab(content, title, plugin);
}

void CoreInterface::removeTab(Core::Tab* tab) {
  tab->close();
}

}  // End namespace Updraft

