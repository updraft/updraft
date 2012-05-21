#ifndef UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_
#define UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

#include <QtGui>
#include <osg/Group>
#include <osgEarth/MapNode>

#include "../../coreinterface.h"
#include "menu.h"
#include "tab.h"


namespace Ui { class MainWindow; }

namespace Updraft {

class MapLayerGroup;

namespace Core {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  Menu* getSystemMenu(SystemMenu menu);

  Tab* createTab(QWidget* content, QString title);

  Menu* createMenu(QString title);
  void removeMenu(Menu* menu);
  void setMapWidget(QWidget *widget);
  QWidget* getMapWidget();

  /// This method creates new map layer group.
  /// It is wrapped by CoreInterface::createMapLayerGroup.
  MapLayerGroupInterface* createMapLayerGroup(const QString &title,
    osg::Group* nodeGroup, osgEarth::MapNode* map);

 private slots:
  /// Called when any tab is about to close.
  /// Posts signal to appropriate tab.
  void tabCloseRequested(int index);

  /// Called when tab has been removed from the bottom pane.
  void tabRemoved(QWidget* tab);

  /// Handles switchin an active tab in the bottom pane.
  void tabSwitch(int index);

  /// Handles File->Open... menu action.
  void openFile();

  /// Handles File->Exit menu action.
  void appExit();

  /// Handles Help->User Documentation menu action.
  void openUserDoc();

  /// Handles Help->About... menu action.
  void showAboutDialog();

 protected:
  // TODO(cestmir): Just a temporary method to test context menu
  void contextMenuEvent(QContextMenuEvent* event);

 private:
  /// Adds the standard menu items to menu.
  /// This includes Open File, Exit, Help ...
  void standardMenuItems();

  /// Adjusts the visibility of the bottom pane.
  /// Shows it when there are any tabs to display, otherwise hides it.
  void tabsVisibility();

  Ui::MainWindow *ui;

  Menu* menuFile;
  Menu* menuView;
  Menu* menuTools;
  Menu* menuHelp;

  Menu* menuContext;
  Menu* menuMapObject;

  Tab* activeTab;

  QSet<Menu*> customMenus;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

