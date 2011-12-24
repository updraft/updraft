#include "CSceneViewer.h"
#include "MainWindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   osg::ArgumentParser arguments(&argc, argv);

   //osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
   /*while (arguments.read("--SingleThreaded")) threadingModel = osgViewer::ViewerBase::SingleThreaded;
   while (arguments.read("--CullDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
   while (arguments.read("--DrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::DrawThreadPerContext;
   while (arguments.read("--CullThreadPerCameraDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext;*/

   CMainWindow w/*(threadingModel)*/;
   w.show();
   return a.exec();
}