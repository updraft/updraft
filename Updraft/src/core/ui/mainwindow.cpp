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

    menuFile = new Menu(ui->menuFile);
    menuEdit = new Menu(ui->menuEdit);
    menuTools = new Menu(ui->menuTools);
    menuHelp = new Menu(ui->menuHelp);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), // NOLINT
      this, SLOT(tabClose(int))); // NOLINT
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), // NOLINT
      this, SLOT(tabSwitch(int))); // NOLINT
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

/// Create a new tab in the bottom pane.
/// Takes ownership of content.
/// \param plugin
Core::Tab* MainWindow::createTab(QWidget* content, QString title,
  IPlugin* plugin) {
  return new Core::Tab(content, title, ui->tabWidget, plugin);
}

void MainWindow::tabClose(int index) {
  QWidget* tab = ui->tabWidget->widget(index);
  delete static_cast<Tab*>(tab);
}

void MainWindow::tabSwitch(int index) {
  if (activeTab) {
    activeTab->deselected();
  }

  if (index == -1) {
    activeTab = NULL;
  } else {
    activeTab = static_cast<Tab*>(ui->tabWidget->widget(index));
    activeTab->selected();
  }
}

}  // End namespace Core
}  // End namespace Updraft

