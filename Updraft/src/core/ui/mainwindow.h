#ifndef UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_
#define UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

#include <QMainWindow>
#include "../../coreinterface.h"
#include "../sceneviewer.h"

namespace Ui { class MainWindow; }

namespace Updraft {

class IPlugin;

namespace Core {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  Menu* getSystemMenu(SystemMenu menu);

  Tab* createTab(QWidget* content, QString title, IPlugin* plugin);

 private slots:
  /// Called when tab in the bottom pane is closed using the little cross
  void tabClose(int index);

  /// Handles switchin an active tab in the bottom pane.
  void tabSwitch(int index);
  
  /// Forces the map drawing.
  void doUpdate();
  
 private:
  Ui::MainWindow *ui;

  Menu* menuFile;
  Menu* menuEdit;
  Menu* menuTools;
  Menu* menuHelp;

  Tab* activeTab;
  
  /// Map view.
  SceneViewer *sceneViewer;
  QTimer* timer;
  
  virtual void paintEvent( QPaintEvent* event );
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

