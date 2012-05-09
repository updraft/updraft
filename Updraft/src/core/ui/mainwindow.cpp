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
  menuView = new Menu(ui->menuView, false);
  menuTools = new Menu(ui->menuTools, false);
  menuHelp = new Menu(ui->menuHelp, false);

  connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
    this, SLOT(tabCloseRequested(int)));
  connect(ui->tabWidget, SIGNAL(currentChanged(int)),
    this, SLOT(tabSwitch(int)));
  tabsVisibility();

  // TODO(cestmir): This is here just to be able to test context menu.
  QMenu* qContextMenu = new QMenu();
  menuContext = new Menu(qContextMenu, true);

  qContextMenu = new QMenu();
  menuMapObject = new Menu(qContextMenu, true);

  standardMenuItems();
}

MainWindow::~MainWindow() {
    delete menuHelp;
    delete menuTools;
    delete menuView;
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
    case MENU_VIEW:
      return menuView;
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

  connect(ret, SIGNAL(tabRemoved(QWidget*)), this, SLOT(tabRemoved(QWidget*)));

  tabsVisibility();

  return ret;
}

void MainWindow::tabCloseRequested(int index) {
  Tab* tab = static_cast<Tab*>(ui->tabWidget->widget(index));
  emit tab->tabCloseRequested();
}

void MainWindow::tabRemoved(QWidget* tab) {
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
  QWidget* map = getMapWidget();
  if (map->geometry().contains(map->mapFrom(this, event->pos()))) {
    // Quell the event if it falls inside the map widget
    event->ignore();
    return;
  }

  menuContext->getQMenu()->popup(event->globalPos());
}

void MainWindow::standardMenuItems() {
  QAction* openAction = new QAction(tr("&Open File..."), this);
  menuFile->insertAction(0, openAction);
  connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

  QAction* exitAction = new QAction(tr("E&xit"), this);
  // Position Exit action at the end of file menu.
  menuFile->insertAction(100, exitAction);
  connect(exitAction, SIGNAL(triggered()), this, SLOT(appExit()));

  // Inserts separator before Exit action.
  QAction* exitSepAction = new QAction(this);
  exitSepAction->setSeparator(true);
  menuFile->insertAction(99, exitSepAction);

  QAction* userDocAction = new QAction(tr("&User Documentation..."), this);
  menuHelp->insertAction(0, userDocAction);
  connect(userDocAction, SIGNAL(triggered()), this, SLOT(openUserDoc()));

  QAction* aboutAction = new QAction(tr("&About..."), this);
  menuHelp->insertAction(1, aboutAction);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
}

void MainWindow::tabsVisibility() {
  ui->tabWidget->setVisible(ui->tabWidget->count() > 0);
}

void MainWindow::openFile() {
  updraft->fileTypeManager->openFileDialog(tr("Open File..."));
}

void MainWindow::appExit() {
  updraft->exit();
}

void MainWindow::openUserDoc() {
}

void MainWindow::showAboutDialog() {
}

void MainWindow::setMapWidget(QWidget *widget) {
  QLayoutItem* child;
  while ((child = ui->layoutFrame->takeAt(0)) != 0) {
    delete child;
  }
  ui->layoutFrame->addWidget(widget);
}

QWidget* MainWindow::getMapWidget() {
  return (ui->layoutFrame->itemAt(0)->widget());
}

}  // End namespace Core
}  // End namespace Updraft

