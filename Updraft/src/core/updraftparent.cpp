#include "updraftparent.h"

namespace Updraft {
namespace Core {

UpdraftParent::UpdraftParent(MainWindow* setWin)
: win(setWin), pluginManager(this) {
  sceneManager = new SceneManager ("C:/data/arc_imagery-elevation-cache");
  win->setMapWidget(sceneManager->getWidget());
}

}  // End namespace Core
}  // End namespace Updraft

