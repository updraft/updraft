#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgQt/GraphicsWindowQt>

#include "CSceneViewer.h"


CSceneViewer::CSceneViewer(osgViewer::ViewerBase::ThreadingModel threadingModel, osg::Camera* camera, osg::Node* scene)
   : osgViewer::Viewer()
{
   setThreadingModel(threadingModel);
   setCamera( camera );

   setSceneData( scene );
   setCameraManipulator( new osgGA::TrackballManipulator );
}

CSceneViewer::~CSceneViewer()
{
}

QWidget* CSceneViewer::getWidget()
{
   osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( getCamera()->getGraphicsContext() );
   return gw ? gw->getGLWidget() : NULL;
}

void CSceneViewer::redrawScene()
{
   frame();
}