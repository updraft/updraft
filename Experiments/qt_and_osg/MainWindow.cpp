#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QWidget>

#include <osgQt/GraphicsWindowQt>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    osg::GraphicsContext::Traits traits;

    osgQt::GraphicsWindowQt *w = new osgQt::GraphicsWindowQt(&traits);

    ui->layout->addWidget(w->getGLWidget());
}

MainWindow::~MainWindow()
{
    delete ui;
}
