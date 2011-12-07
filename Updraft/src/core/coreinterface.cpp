#include "../coreinterface.h"
#include "updraftparent.h"
#include "ui/mainwindow.h"

namespace Updraft {

QMainWindow* CoreInterface::getMainWindow() {
  return qobject_cast<QMainWindow*>(parent->getMainWindow());
}

Core::Menu* CoreInterface::getSystemMenu(SystemMenu menu) {
  Core::MainWindow* win = parent->getMainWindow();
  return win->getSystemMenu(menu);
}

}  // End namespace Updraft

