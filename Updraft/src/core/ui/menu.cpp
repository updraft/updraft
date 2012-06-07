#include "menu.h"
#include <QtGui>

namespace Updraft {
namespace Core {

Menu::~Menu() {
  if (ownsMenu) {
    delete menu;
  }

  foreach(QAction* action, ownedActions) {
    delete action;
  }
}

void Menu::lightClear() {
  menu->clear();
  actions.clear();
}

void Menu::clear() {
  lightClear();

  foreach(QAction* action, ownedActions) {
    delete action;
  }
  ownedActions.clear();
}

void Menu::insertAction(int position, QAction* action, bool own) {
  actions.insert(position, action);

  if (own) {
    ownedActions.push_back(action);
  }

  reorganizeMenu();
}

void Menu::appendAction(QAction* action, bool own) {
  actions.insert(actions.size(), action);

  if (own) {
    ownedActions.push_back(action);
  }

  menu->addAction(action);
}

QMenu* Menu::giveQMenu() {
  QMenu* ret = menu;

  menu = new QMenu();
  actions.clear();

  return ret;
}

void Menu::reorganizeMenu() {
  menu->clear();

  foreach(QAction* action, actions) {
    menu->addAction(action);
  }
}

}  // End namespace Core
}  // End namespace Updraft

