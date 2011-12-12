#ifndef SceneViewer_H
#define SceneViewer_H

#include <QtGui/QWidget>
#include <osgViewer/Viewer>
#include <osgEarth/MapNode>

class SceneViewer : private osgViewer::Viewer
{
 public:
  SceneViewer(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::ViewerBase::SingleThreaded);
  ~SceneViewer();

  QWidget* getWidget();
  void redrawScene();

 private:
  osgEarth::MapNode* mapNode;
  osg::Camera* camera;

  osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
  osg::Group* createScene();
  osgEarth::MapNode* createMap();
};

#endif//SceneViewer_H