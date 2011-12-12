#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <osgViewer/Viewer>
#include "../sceneviewer.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 public slots:
  void doUpdate(); // update osgEarth map

 private:
  Ui::MainWindow *ui;

  /**
  * Map view.
  */
  SceneViewer *sceneViewer;
  QTimer* timer;

  virtual void paintEvent( QPaintEvent* event );
};

#endif // MAINWINDOW_H
