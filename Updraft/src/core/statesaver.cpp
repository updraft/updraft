#include "statesaver.h"

#include "updraft.h"
#include "ui/mainwindow.h"

namespace Updraft {
namespace Core {

StateSaver::StateSaver() {
  updraft->settingsManager->addGroup("state", "Application state", "");

  windowGeometry = updraft->settingsManager->addSetting(
    "state:geometry", "Main window geometry", QByteArray(), true);
}

void StateSaver::save() {
  windowGeometry->set(updraft->mainWindow->saveGeometry());
}

void StateSaver::load() {
  updraft->mainWindow->restoreGeometry(windowGeometry->get().toByteArray());
}

}  // End namespace Core
}  // End namespace Updraft

