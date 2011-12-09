#ifndef UPDRAFT_CORE_INTERFACE_H_
#define UPDRAFT_CORE_INTERFACE_H_

#include "core/ui/menu.h"
#include "core/ui/tab.h"

class QWidget;
class QMainWindow;

namespace Updraft {

//Forward declarations
class IPlugin;
namespace Core {
  class UpdraftParent;
  class Menu;
} //End namespace Core

enum SystemMenu {
  MENU_FILE,
  MENU_EDIT,
  MENU_TOOLS,
  MENU_HELP,
  MENU_CONTEXT
};

class CoreInterface {
  /// \todo Fill this once plugin API is finished
 public:
  CoreInterface(): plugin(NULL), parent(NULL) {}
  CoreInterface(Core::UpdraftParent* par, IPlugin *pl) :
    plugin(pl), parent(par) {}

  /// Returns main window pointer. Used for example for modal dialogs
  QMainWindow* getMainWindow();

  /// Checks, whether the class was correctly constructed.
  bool valid() { return (plugin != NULL && parent != NULL); }

  /// Returns pointer to the instance of a system menu
  /// \param menu which system menu instance to return  
  Core::Menu* getSystemMenu(SystemMenu menu);

  /// Open a Tab with any widget.
  Core::Tab* createTab(QWidget* content, QString title);

  /// Removes tab from the bottom pane
  /// The tab class given to this function is deleted!
  void removeTab(Core::Tab* tab);
  
 private:
  IPlugin* plugin;
  Core::UpdraftParent* parent;
};

}  // End namespace Updraft

#endif

