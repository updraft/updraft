#include "menu.h"
#include <QtGui>

namespace Updraft {
namespace Core {

void Menu::insertAction(int position, QAction* action) {
  // TODO(cestmir): use the position argument
  menu->addAction(action);
}

}  // End namespace Core
}  // End namespace Updraft

