#ifndef UPDRAFT_SRC_CORE_UI_MENU_H_
#define UPDRAFT_SRC_CORE_UI_MENU_H_

#include <QtGui>

#include "../../menuinterface.h"

class QMenu;
class QAction;

namespace Updraft {
namespace Core {


/// Wrapper around QMenu that can work with item priorities.
class Menu : public MenuInterface {
  // TODO(cestmir): We should take care about the destruction of parent menu.
  // So far, it's not a problem because we don't create the menus dynamically
 public:
  /// Constructor
  /// \param setMenu The QMenu with which the menu is initialized
  /// \param ownsQMenu Whether the QMenu passed to this constructor should
  ///                  be destoryed when the Menu instance is destroyed.
  ///                  Default: false
  Menu(QMenu* setMenu, bool ownsQMenu)
    : menu(setMenu), ownsMenu(ownsQMenu) {}

  ~Menu();

  /// Clears the actions from the menu, but does not destroy the
  /// owned actions.
  void lightClear();

  /// Clears the whole menu and destroys the owned actions
  void clear();

  /// Insert action into this menu.
  /// \param position Priority of this action. Actions with
  ///                 lower value are closer to the top.
  /// \param action The action to be inserted
  /// \param own Whether the action's ownership should transfer to the menu.
  ///            If true, the menu destroys the QAction when it is no longer
  ///            needed.
  void insertAction(int position, QAction* action, bool own = false);

  /// Appends an action into the menu.
  /// \param action The action to be appended
  /// \param own Whether the action's ownership should transfer to the menu.
  ///            If true, the menu destroys the QAction when it is no longer
  ///            needed.
  void appendAction(QAction* action, bool own = false);

  /// Gets the QMenu wrapped by this menu.
  /// \return The QMenu wrapped by this menu
  QMenu* getQMenu() { return menu; }

  /// Returns the contained QMenu and creates a new empty one.
  /// The ownership of the menu is transferred to the caller.
  QMenu* giveQMenu();

 private:
  QMenu* menu;
  bool ownsMenu;

  /// All the actions sorted by priority. Used for inserting a new action
  QMultiMap<int, QAction*> actions;

  /// Actions that this menu owns and should be erased when the menu is erased.
  QList<QAction*> ownedActions;

  /// Called upon insertion or removal of an action
  void reorganizeMenu();
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MENU_H_

