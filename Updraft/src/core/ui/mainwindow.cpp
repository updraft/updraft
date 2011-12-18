#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menu.h"

namespace Updraft {
namespace Core {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      activeTab(NULL) {
    ui->setupUi(this);

    menuFile = new Menu(ui->menuFile, false);
    menuEdit = new Menu(ui->menuEdit, false);
    menuTools = new Menu(ui->menuTools, false);
    menuHelp = new Menu(ui->menuHelp, false);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
      this, SLOT(tabClose(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
      this, SLOT(tabSwitch(int)));

    // TODO(cestmir): This is here just to be able to test context menu.
    QMenu* qContextMenu = new QMenu();
    menuContext = new Menu(qContextMenu, true);
}

MainWindow::~MainWindow() {
    delete menuHelp;
    delete menuTools;
    delete menuEdit;
    delete menuFile;
    delete menuContext;
    delete ui;

    // TODO(Kuba): Delete the built-in menus together with custom menus?
    foreach(Menu* menu, customMenus) {
        delete menu;
    }
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
    case MENU_CONTEXT:
    default:
      return menuContext;
    break;
  }
}

/// Create a new tab in the bottom pane.
/// Takes ownership of content.
Core::Tab* MainWindow::createTab(QWidget* content, QString title) {
  return new Core::Tab(content, title, ui->tabWidget);
}

void MainWindow::tabClose(int index) {
  QWidget* tab = ui->tabWidget->widget(index);
  delete static_cast<Tab*>(tab);
}

void MainWindow::tabSwitch(int index) {
  if (activeTab) {
    emit activeTab->deselected();
  }

  if (index == -1) {
    activeTab = NULL;
  } else {
    activeTab = static_cast<Tab*>(ui->tabWidget->widget(index));
    emit activeTab->selected();
  }
}

Menu* MainWindow::createMenu(QString title) {
  QMenu* qMenu = new QMenu(title, ui->menuBar);
  Menu* newMenu = new Menu(qMenu, true);  // Make the qmenu owned by our Menu

  customMenus.insert(newMenu);
  ui->menuBar->addMenu(qMenu);

  return newMenu;
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {
  menuContext->getQMenu()->popup(event->globalPos());
}

}  // End namespace Core
}  // End namespace Updraft

