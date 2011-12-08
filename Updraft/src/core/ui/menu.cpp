#include "menu.h"
#include <QtGui>

namespace Updraft {
namespace Core {

void Menu::insertAction(int position, QAction* action) {
  actions.insert(position, action);

  reorganizeMenu();
}

void Menu::reorganizeMenu() {
  menu->clear();

  QList<QAction*> actionList = actions.values();
  foreach(QAction* action, actionList) {
    menu->addAction(action);
  }
}

}  // End namespace Core
}  // End namespace Updraft

