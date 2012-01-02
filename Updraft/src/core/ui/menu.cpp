#include "menu.h"
#include <QtGui>

namespace Updraft {
namespace Core {

Menu::~Menu() {
  if (ownsMenu) {
    delete menu;
  }
}

/// Insert action into this menu.
/// \param position Priority of this action. Actions with
///   lower value are closer to the top.
void Menu::insertAction(int position, QAction* action) {
  actions.insert(position, action);

  reorganizeMenu();
}

void Menu::reorganizeMenu() {
  menu->clear();

  foreach(QAction* action, actions) {
    menu->addAction(action);
  }
}

}  // End namespace Core
}  // End namespace Updraft

