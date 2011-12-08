#include "../coreinterface.h"
#include "updraftparent.h"
#include "ui/mainwindow.h"

namespace Updraft {

using Core::MainWindow;

QMainWindow* CoreInterface::getMainWindow() {
  return qobject_cast<QMainWindow*>(parent->getMainWindow());
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

}  // End namespace Updraft

