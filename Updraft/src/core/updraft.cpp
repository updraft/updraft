#include "updraft.h"

namespace Updraft {
namespace Core {

Updraft::Updraft(int &argc, char** argv) {
  QTranslator trans;
  trans.load("translations/czech");

  installTranslator(&trans);

  mainWindow = new MainWindow(NULL);
  pluginManager = new PluginManager();
  sceneManager = new SceneManager("data/initial.earth");

  win->setMapWidget(sceneManager->getWidget());
}

Updraft::~Updraft() {
  delete sceneManager;
  delete pluginManager;
  delete mainWindow;
}

}  // End namespace Core
}  // End namespace Updraft

