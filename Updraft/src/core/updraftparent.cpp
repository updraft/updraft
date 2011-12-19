#include "updraftparent.h"

namespace Updraft {
namespace Core {

UpdraftParent::UpdraftParent(MainWindow* setWin)
: win(setWin), pluginManager(this) {
  sceneManager = new SceneManager("data/initial.earth");
  win->setMapWidget(sceneManager->getWidget());
}

}  // End namespace Core
}  // End namespace Updraft

