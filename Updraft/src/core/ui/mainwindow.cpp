#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menu.h"

namespace Updraft {
namespace Core {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {
    ui->setupUi(this);

    menuFile = new Menu(ui->menuFile);
    menuEdit = new Menu(ui->menuEdit);
    menuTools = new Menu(ui->menuTools);
    menuHelp = new Menu(ui->menuHelp);
}

MainWindow::~MainWindow() {
    delete menuHelp;
    delete menuTools;
    delete menuEdit;
    delete menuFile;
    delete ui;
}

Menu* MainWindow::getSystemMenu(SystemMenu menu) {
  switch (menu) {
    case MENU_FILE:
      return menuFile;
    break;
    case MENU_EDIT:
      return menuEdit;
    break;
    case MENU_TOOLS:
      return menuTools;
    break;
    case MENU_HELP:
      return menuHelp;
    break;
    default:
      return menuHelp;
    break;
  }
}

Menu* MainWindow::createMenu(QString title) {
  QMenu* qMenu = new QMenu(title, ui->menuBar);
  Menu* newMenu = new Menu(qMenu, true);  // Make the qmenu owned by our Menu

  customMenus.insert(newMenu);
  ui->menuBar->addMenu(qMenu);

  return newMenu;
}

void MainWindow::removeMenu(Menu* menu) {
  if (customMenus.remove(menu)) {
    delete menu;
  }
}

}  // End namespace Core
}  // End namespace Updraft

