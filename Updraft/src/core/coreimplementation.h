#ifndef UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_
#define UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_

#include "../coreinterface.h"
#include "updraftparent.h"

namespace Updraft {

class PluginBase;

namespace Core {

/// Implements the core functionalities avaliable to plugins.
/// There is an instance of this class for every loaded plugin.
class CoreImplementation : public CoreInterface {
 public:
  CoreImplementation(UpdraftParent* par, PluginBase *pl);

  QMainWindow* getMainWindow();

  Menu* createMenu(QString title);
  Menu* getSystemMenu(SystemMenu menu);

  TabInterface* createTab(QWidget* content, QString title);
  void removeTab(TabInterface* tab);

 private:
  PluginBase* plugin;
  UpdraftParent* parent;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_COREIMPLEMENTATION_H_

