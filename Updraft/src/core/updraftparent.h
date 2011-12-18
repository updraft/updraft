#ifndef UPDRAFT_SRC_CORE_UPDRAFTPARENT_H_
#define UPDRAFT_SRC_CORE_UPDRAFTPARENT_H_

#include "pluginmanager.h"
#include "scenemanager.h"
#include "ui/mainwindow.h"

namespace Updraft {
namespace Core {

/// Class that takes care of all the important objects in Updraft
class UpdraftParent {
 public:
  explicit UpdraftParent(MainWindow* setWin);
  ~UpdraftParent() {}

  MainWindow* getMainWindow() { return win; }

 private:
  // Important: MainWindow has to initialize before pluginManager (because
  // loading plugins can reference the main window) Therefore, this member
  // has to come before pluginManager.
  MainWindow* win;

  PluginManager pluginManager;
  SceneManager* sceneManager;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UPDRAFTPARENT_H_

