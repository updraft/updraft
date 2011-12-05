#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "OsgViewerWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OsgViewerWidget *ovw = new OsgViewerWidget();

    ui->verticalLayout->addWidget(ovw);

    ovw->root->addChild(
                osgDB::readNodeFile("/home/cube/updraft/Experiments/qt_and_osg/cessna.osgt")
                );

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonClicked()
{

}
