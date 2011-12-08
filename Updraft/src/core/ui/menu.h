#ifndef UPDRAFT_SRC_CORE_UI_MENU_H_
#define UPDRAFT_SRC_CORE_UI_MENU_H_

#include <QtGui>

class QMenu;
class QAction;

namespace Updraft {
namespace Core {

// TODO(cestmir): We should take care about the destruction of the parent menu.
// So far, it's not a problem because we don't create the menus dynamically
class Menu {
 public:
  /// Constructor
  /// \param ownsQMenu Whether the QMenu passed to this constructor should
  //                   be destoryed when the Menu instance is destroyed.
  //                   Default: false
  explicit Menu(QMenu* setMenu, bool ownsQMenu = false)
  : menu(setMenu), ownsMenu(ownsQMenu) {}

  ~Menu();

  void insertAction(int position, QAction* action);

  QMenu* getQMenu() { return menu; }
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

