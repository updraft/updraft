#ifndef UPDRAFT_SRC_MENUINTERFACE_H_
#define UPDRAFT_SRC_MENUINTERFACE_H_

class QAction;

namespace Updraft {

enum SystemMenu {
  MENU_FILE,
  MENU_EDIT,
  MENU_TOOLS,
  MENU_HELP,
  MENU_CONTEXT
};

/// Interface to menu used by plugins.
class MenuInterface {
 public:
  /// Insert action into this menu.
  /// Takes ownership of action.
  /// \param position Priority of this action. Actions with
  ///   lower value are closer to the top.
  virtual void insertAction(int position, QAction* action) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_MENUINTERFACE_H_

