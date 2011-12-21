#ifndef UPDRAFT_SRC_CORE_UPDRAFT_H_
#define UPDRAFT_SRC_CORE_UPDRAFT_H_

#include <QApplication>

/// Pointer to the instance of Updraft object.
#define updraft (static_cast<Updraft*>(Updraft::instance()))

namespace Updraft {
namespace Core {

/// Top level object of updraft project.
class Updraft : public QApplication {
 public:
  Updraft(int &argc, char** argv);
  ~Updraft();

  MainWindow* mainWindow;
  PluginManager* pluginManager;
  SceneManager* sceneManager;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UPDRAFT_H_

