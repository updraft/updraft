#ifndef __SCENEVIEWWIDGET_H_
#define __SCENEVIEWWIDGET_H_

#include <QtGui/QWidget>
#include <osgViewer/Viewer>

class CSceneViewer : private osgViewer::Viewer
{
   public:
      CSceneViewer(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::ViewerBase::SingleThreaded, osg::Camera* camera=NULL, osg::Node* scene=NULL);
      ~CSceneViewer();

      QWidget* getWidget();
      void redrawScene();
};

#endif//__SCENEVIEWWIDGET_H_