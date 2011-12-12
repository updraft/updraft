#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent): QMainWindow(), sceneViewer(NULL),
    ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // add osgEarth viewer into the layout
  sceneViewer = new SceneViewer ();  
  ui->layoutFrame->addWidget (sceneViewer->getWidget());

  // set timer to update map view
  timer = new QTimer ();
  connect( timer, SIGNAL(timeout()), this, SLOT(doUpdate()));
  timer->start(10);
}

MainWindow::~MainWindow() {
   delete ui;
}

void MainWindow::doUpdate() {
  update();
}

void MainWindow::paintEvent( QPaintEvent* event ) {
  sceneViewer->redrawScene();
}
