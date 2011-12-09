#ifndef __SCENEVIEWWIDGET_H_
#define __SCENEVIEWWIDGET_H_

#include <QtGui/QWidget>
#include <osgViewer/Viewer>
#include <osgEarth/MapNode>

class CSceneViewer : private osgViewer::Viewer
{
   public:
      CSceneViewer(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::ViewerBase::SingleThreaded, osg::Camera* camera=NULL, osg::Node* scene=NULL);
      ~CSceneViewer();

      QWidget* getWidget();
      void redrawScene();
private:
	osgEarth::MapNode* mapNode;
};

#endif//__SCENEVIEWWIDGET_H_