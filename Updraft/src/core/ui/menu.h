#ifndef UPDRAFT_SRC_CORE_UI_MENU_H_
#define UPDRAFT_SRC_CORE_UI_MENU_H_

class QMenu;
class QAction;

namespace Updraft {
namespace Core {

// TODO(cestmir): We should take care about the destruction of the parent menu.
// So far, it's not a problem because we don't create the menus dynamically
class Menu {
 public:
  explicit Menu(QMenu* setMenu): menu(setMenu) {}

  void insertAction(int position, QAction* action);
 private:
  QMenu* menu;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MENU_H_

