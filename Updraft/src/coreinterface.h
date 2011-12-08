#ifndef UPDRAFT_CORE_INTERFACE_H_
#define UPDRAFT_CORE_INTERFACE_H_

#include <cstddef>
#include "core/ui/menu.h"

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

  /// Create an entry in the main menu.
  Core::Menu* createMenu(QString title);

  /// Remove an entry from the main menu
  void removeMenu(Core::Menu* menu);

  /// Returns pointer to the instance of a system menu
  /// \param menu which system menu instance to return  
  Core::Menu* getSystemMenu(SystemMenu menu);

 private:
  IPlugin* plugin;
  Core::UpdraftParent* parent;
};

}  // End namespace Updraft

#endif

