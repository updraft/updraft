#include "updraftparent.h"

namespace Updraft {
namespace Core {

UpdraftParent::UpdraftParent(MainWindow* setWin)
: win(setWin), pluginManager(this) {
  sceneManager = new SceneManager("data/arc_imagery-elevation-cache.earth");
  win->setMapWidget(sceneManager->getWidget());
}

}  // End namespace Core
}  // End namespace Updraft

