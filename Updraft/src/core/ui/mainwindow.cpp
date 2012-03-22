#include "mainwindow.h"

#include <QFileDialog>

#include "ui_mainwindow.h"

#include "menu.h"
#include "maplayergroup.h"
#include "updraft.h"

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
  tabsVisibility();

  // TODO(cestmir): This is here just to be able to test context menu.
  QMenu* qContextMenu = new QMenu(this);
  menuContext = new Menu(qContextMenu, true);

  qContextMenu = new QMenu(this);
  menuMapObject = new Menu(qContextMenu, true);

  standardMenuItems();
}

MainWindow::~MainWindow() {
    delete menuHelp;
    delete menuTools;
    delete menuEdit;
    delete menuFile;
    delete menuContext;
    delete menuMapObject;
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
    case MENU_MAPOBJECT:
      return menuMapObject;
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
  Core::Tab* ret = new Core::Tab(content, title, ui->tabWidget);

  tabsVisibility();

  return ret;
}

void MainWindow::tabClose(int index) {
  QWidget* tab = ui->tabWidget->widget(index);
  static_cast<Tab*>(tab)->close();
  tabsVisibility();
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

MapLayerGroupInterface* MainWindow::createMapLayerGroup(const QString &title,
  osg::Group* nodeGroup, osgEarth::MapNode* map) {
  return new MapLayerGroup(ui->treeWidget_2, title, nodeGroup, map);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {
  menuContext->getQMenu()->popup(event->globalPos());
}

/// Add the standard menu items to menu.
/// This includes Open / Save, Help ...
void MainWindow::standardMenuItems() {
  QAction* openAction = new QAction(tr("&Open File..."), this);
  menuFile->insertAction(0, openAction);
  connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
}

void MainWindow::tabsVisibility() {
  ui->tabWidget->setVisible(ui->tabWidget->count() > 0);
}

/// Handle File->Open... menu action.
void MainWindow::openFile() {
  updraft->fileTypeManager->openFileDialog(tr("Open File..."));
}

void MainWindow::setMapWidget(QWidget *widget) {
  QLayoutItem* child;
  while ( (child = ui->layoutFrame->takeAt(0)) != 0) {
    delete child;
  }
  ui->layoutFrame->addWidget(widget);
}

QWidget* MainWindow::getMapWidget() {
  return (ui->layoutFrame->itemAt(0)->widget());
}

}  // End namespace Core
}  // End namespace Updraft

