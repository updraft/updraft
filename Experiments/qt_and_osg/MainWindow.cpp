#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QWidget>

#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osgDB/ReadFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits();

    osgQt::GraphicsWindowQt *w = new osgQt::GraphicsWindowQt(traits);

    ui->layout->addWidget(w->getGLWidget());

    osg::Camera *cam = new osg::Camera();

    cam->setGraphicsContext(w);
    cam->setViewport(0, 0, w->getGLWidget()->width(), w->getGLWidget()->height());

    osgViewer::Viewer *viewer = new osgViewer::Viewer();

    //cam->setView(viewer);
    viewer->addSlave(cam);
    viewer->setThreadingModel(osgViewer::Viewer::AutomaticSelection);

    //osgQt::setViewer(viewer);

    osg::Node *n = osgDB::readNodeFile("/home/cube/updraft/Experiments/qt_and_osg/cessna.osgt");

    viewer->setSceneData(n);
    viewer->run();

}

MainWindow::~MainWindow()
{
    delete ui;
}
