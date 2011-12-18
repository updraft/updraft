#ifndef UPDRAFT_SRC_CORE_SCENEMANAGER_H_
#define UPDRAFT_SRC_CORE_SCENEMANAGER_H_

#include <QtGui/QWidget>
#include <osgViewer/Viewer>
#include <QTimer>
#include "mapmanager.h"

namespace Updraft {
namespace Core {

class SceneManager: public QObject {
 
 Q_OBJECT

 public:
  SceneManager(QString baseEarthFile, osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::ViewerBase::SingleThreaded);
  ~SceneManager();

  QWidget* getWidget();
  osg::Group* getRoot();
  MapManager* getMapManager();

 public slots:
  void redrawScene();

 private:
  osgViewer::Viewer* viewer;
  osg::Group* sceneRoot;
  MapManager* mapManager;
  osg::Camera* camera;
  QTimer* timer;

  osg::Camera* createCamera(int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);
  osg::Group* createInitialScene(osgEarth::MapNode* mapNode);
};

} // end namespace Core
} // end namespace Updraft

#endif // UPDRAFT_SRC_CORE_SCENEMANAGER_H
