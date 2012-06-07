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

namespace Core {

class MapLayerGroup;
class MapLayer;

/// Main application window.
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

  /// Returns the invisible top level MapLayerGroup.
  MapLayerGroup* getInvisibleRootMapLayerGroup();

  /// Add a map layer and its tree widget item to the list of map layers.
  void registerMapLayer(MapLayerInterface* layer);

  /// Remove a map layer and its tree widget item from the list of map layers.
  void unregisterMapLayer(MapLayerInterface* layer);

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

  /// Item in map layer tree widget was changed (= checked / unchecked)
  void mapLayerItemChanged(QTreeWidgetItem *item);

  /// Map layer tree view requested context menu.
  /// Finds thee map layer and send the event to it.
  void mapLayerContextMenuRequested(const QPoint& pos);

 protected:
  // TODO(cestmir): Just a temporary method to test context menu
  void contextMenuEvent(QContextMenuEvent* event);

 private:
  /// Adds the standard menu items to menu.
  /// This includes Open File, Exit, Help ...
  void standardMenuItems();

  MapLayerGroup *invisibleRootMapLayerGroup;

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

  QMap<QTreeWidgetItem*, MapLayerInterface *> mapLayers;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

