#ifndef UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_
#define UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

#include <QMainWindow>
#include "../../coreinterface.h"

namespace Ui {
  class MainWindow;
}

namespace Updraft {
namespace Core {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  Menu* getSystemMenu(SystemMenu menu);

 private:
  Ui::MainWindow *ui;

  Menu* menuFile;
  Menu* menuEdit;
  Menu* menuTools;
  Menu* menuHelp;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

