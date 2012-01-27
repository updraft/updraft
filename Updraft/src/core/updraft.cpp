#include "updraft.h"
#include "ui/maplayergroup.h"

namespace Updraft {
namespace Core {

Updraft::Updraft(int argc, char** argv)
  : QApplication(argc, argv) {
  QTranslator trans;
  trans.load("translations/czech");

  installTranslator(&trans);

  mainWindow = new MainWindow(NULL);
  fileTypeManager = new FileTypeManager();
  sceneManager = new SceneManager(
    QCoreApplication::applicationDirPath() + "/data/initial.earth");
  pluginManager = new PluginManager();

  mainWindow->setMapWidget(sceneManager->getWidget());
}

Updraft::~Updraft() {
  delete sceneManager;
  delete pluginManager;
  delete fileTypeManager;
  delete mainWindow;
}

QString Updraft::getDataDirectory() {
  return QCoreApplication::applicationDirPath() + "/data";
}

/// Pull the lever.
/// Shows main window, and enters event loop.
int Updraft::exec() {
  mainWindow->show();
  return QApplication::exec();
}

}  // End namespace Core
}  // End namespace Updraft

