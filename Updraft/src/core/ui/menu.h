#ifndef UPDRAFT_SRC_CORE_UI_MENU_H_
#define UPDRAFT_SRC_CORE_UI_MENU_H_

#include <QtGui>

#include "../../menuinterface.h"

class QMenu;
class QAction;

namespace Updraft {
namespace Core {

// TODO(cestmir): We should take care about the destruction of the parent menu.
// So far, it's not a problem because we don't create the menus dynamically
class Menu : public MenuInterface {
 public:
  /// Constructor
  /// \param ownsQMenu Whether the QMenu passed to this constructor should
  //                   be destoryed when the Menu instance is destroyed.
  //                   Default: false
  explicit Menu(QMenu* setMenu, bool ownsQMenu)
  : menu(setMenu), ownsMenu(ownsQMenu) {}

  ~Menu();

  void clear();

  void insertAction(int position, QAction* action);
  void appendAction(QAction* action);

  QMenu* getQMenu() { return menu; }

  /// Returns the contained QMenu and creates a new empty one.
  /// The ownership of the menu is transferred to the caller.
  QMenu* giveQMenu();

 private:
  QMenu* menu;
  bool ownsMenu;

  /// All the actions sorted by priority. Used for inserting a new action
  QMultiMap<int, QAction*> actions;

  /// Called upon insertion or removal of an action
  void reorganizeMenu();
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MENU_H_

