#include "statesaver.h"

#include "updraft.h"
#include "ui/mainwindow.h"
#include "filetypemanager.h"

namespace Updraft {
namespace Core {

StateSaver::StateSaver() {
  updraft->settingsManager->addGroup("state", "Application state", "");

  windowGeometry = updraft->settingsManager->addSetting(
    "state:geometry", "Main window geometry", QByteArray(), true);
  openDirectory = updraft->settingsManager->addSetting(
    "state:openDirectory", "File open dialog directory",
    QVariant(), true);
}

void StateSaver::save() {
  QVariant x;

  windowGeometry->set(updraft->mainWindow->saveGeometry());

  x.setValue(updraft->fileTypeManager->lastDirectory());
  openDirectory->set(x);
}

void StateSaver::load() {
  QVariant x;

  updraft->mainWindow->restoreGeometry(windowGeometry->get().toByteArray());

  x = openDirectory->get();
  updraft->fileTypeManager->setLastDirectory(x.value<QDir>());
}

}  // End namespace Core
}  // End namespace Updraft

