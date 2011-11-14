#include "OsgViewerWidget.h"

#include <QGridLayout>

#include <stdio.h>

OsgViewerWidget::OsgViewerWidget(QWidget *parent) :
    QWidget(parent)
{
    osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits();

    osgQt::GraphicsWindowQt *w = new osgQt::GraphicsWindowQt(traits);

    camera = new osg::Camera();
    camera->setGraphicsContext(w);
    camera->setViewport(0, 0, width(), height());
    camera->setProjectionMatrixAsPerspective(
                30, width() / (double)height(), 1, 10000);

    viewer = new osgViewer::Viewer();
    viewer->setCamera(camera);
    viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
        // There's a problem with multi threaded model on linux
        // http://forum.openscenegraph.org/viewtopic.php?t=9055
        // https://bugreports.qt.nokia.com/browse/QTBUG-200

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(w->getGLWidget(), 0, 0);
    setLayout(grid);
}

OsgViewerWidget::~OsgViewerWidget()
{
}

void OsgViewerWidget::paintEvent(QPaintEvent *)
{
    printf("paintEvent\n");
    viewer->frame();
}
