#ifndef OSGVIEWER_H
#define OSGVIEWER_H

#include <QWidget>

#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osgDB/ReadFile>


class OsgViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OsgViewerWidget(QWidget *parent = 0);
    ~OsgViewerWidget();

    void paintEvent(QPaintEvent *);

    osgViewer::Viewer *viewer;
    osg::Camera *camera;
};

#endif // OSGVIEWER_H