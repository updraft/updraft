#ifndef UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_
#define UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
};

#endif  // UPDRAFT_SRC_CORE_UI_MAINWINDOW_H_

