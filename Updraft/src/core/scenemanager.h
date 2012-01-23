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

/// SceneManager class is a wrapper of the scene, and the scene graph.
class SceneManager: public QObject {
  Q_OBJECT

 public:
  SceneManager(QString baseEarthFile,
      osgViewer::ViewerBase::ThreadingModel threadingModel =
      osgViewer::ViewerBase::SingleThreaded);
  ~SceneManager();

  /// Returns the drawing widget.
  QWidget* getWidget();

  /// Returns the root of the scene.
  osg::Group* getRoot();

  /// Returns the map node in the scene.
  /// Note that there must be only one map node in the scene.
  osgEarth::MapNode* getMapNode();

  /// Returns the MapManager instance associated with the map.
  MapManager* getMapManager();

  /// Creates an empty osg::Group, and adds it to the root.
  /// There is always one group associated with one
  /// MapLayerGroup.
  osg::Group* newGroup();

  /// Removes the group from the root.
  bool removeGroup(osg::Group* group);

 public slots:
  void redrawScene();

 private:
  osgViewer::Viewer* viewer;
  osg::Group* sceneRoot;

  /// Node with the background.
  osg::ClearNode* background;
  osgEarth::MapNode* mapNode;

  MapManager* mapManager;
  osg::Camera* camera;
  osgQt::GraphicsWindowQt* graphicsWindow;

  /// Timer that triggers the drawing procedure.
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
