#include "OsgViewerWidget.h"

#include <QGridLayout>
#include <QEvent>
#include <QDebug>

#include <stdio.h>

#include <osg/DisplaySettings>
#include <osgGA/TrackballManipulator>

OsgViewerWidget::OsgViewerWidget(QWidget *parent) :
    QWidget(parent)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

    osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits();
    traits->windowName = "asdfNameAsdf";
    traits->windowDecoration = false;
    traits->x = 0;
    traits->y = 0;
    traits->width = 100;
    traits->height = 100;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    osgQt::GraphicsWindowQt *w = new osgQt::GraphicsWindowQt(traits);

    camera = new osg::Camera();
    camera->setGraphicsContext(w);
    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(0, 0, traits->width, traits->height);
    camera->setProjectionMatrixAsPerspective(
                30.0f, traits->width / (double)traits->width, 1.0f, 10000.0f);

    viewer = new osgViewer::Viewer(); // osgViewer::Viewer is a subclass of osgViewer::View
    viewer->setCamera(camera);
    viewer->setCameraManipulator( new osgGA::TrackballManipulator );
    viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
        // There's a problem with multi threaded model on linux
        // http://forum.openscenegraph.org/viewtopic.php?t=9055
        // https://bugreports.qt.nokia.com/browse/QTBUG-200


    QGridLayout* grid = new QGridLayout();
    setLayout(grid);
    //grid->setContentsMargins(0, 0, 0, 0);
    grid->addWidget(w->getGLWidget(), 0, 0);
}

OsgViewerWidget::~OsgViewerWidget()
{
}

void OsgViewerWidget::paintEvent(QPaintEvent *)
{
    qDebug() << "paintEvent";
    viewer->frame();
}
