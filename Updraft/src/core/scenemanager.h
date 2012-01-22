#ifndef UPDRAFT_SRC_CORE_SCENEMANAGER_H_
#define UPDRAFT_SRC_CORE_SCENEMANAGER_H_

#include <QtGui/QWidget>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>
#include <QTimer>
#include <string>
#include "mapmanager.h"

namespace Updraft {
namespace Core {

class SceneManager: public QObject {
  Q_OBJECT

 public:
  SceneManager(QString baseEarthFile,
      osgViewer::ViewerBase::ThreadingModel threadingModel =
      osgViewer::ViewerBase::SingleThreaded);
  ~SceneManager();

  QWidget* getWidget();
  osg::Group* getRoot();
  osgEarth::MapNode* getMapNode();
  MapManager* getMapManager();
  osg::Group* newGroup();
  bool removeGroup(osg::Group* group);

 public slots:
  void redrawScene();

 private:
  osgViewer::Viewer* viewer;
  osg::Group* sceneRoot;
  osg::ClearNode* background;
  osgEarth::MapNode* mapNode;

  MapManager* mapManager;
  osg::Camera* camera;
  osgQt::GraphicsWindowQt* graphicsWindow;
  QTimer* timer;

  osg::GraphicsContext::Traits* createGraphicsTraits
    (int x, int y, int w, int h, const std::string& name = "",
    bool windowDecoration = false);
  osgQt::GraphicsWindowQt* createGraphicsWindow
    (osg::GraphicsContext::Traits* traits);
  osg::Camera* createCamera(osg::GraphicsContext::Traits* traits);
};

}  // end namespace Core
}  // end namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SCENEMANAGER_H_
