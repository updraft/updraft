#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <osgViewer/Viewer>

#include "ui_qt_and_osg_with_gui.h"

class CSceneViewer;

class CMainWindow : public QMainWindow
{
   Q_OBJECT

public:
   CMainWindow(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::ViewerBase::SingleThreaded);
   ~CMainWindow();

private:
   Ui_MainWindow ui;
   CSceneViewer *sceneViewer;

   osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
   virtual void paintEvent( QPaintEvent* event );
};

#endif//__MAINWINDOW_H_