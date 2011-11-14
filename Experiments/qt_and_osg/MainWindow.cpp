#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "OsgViewerWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OsgViewerWidget *ovw = new OsgViewerWidget();

    ui->horizontalLayout->addWidget(ovw);

    ovw->viewer->setSceneData(
                osgDB::readNodeFile("/home/cube/updraft/Experiments/qt_and_osg/cessna.osgt")
                );

    ovw->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
