#include <osgQt/GraphicsWindowQt>
#include <osgEarth/ViewPoint>
#include <osgEarthUtil/ObjectPlacer>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <string>

#include "scenemanager.h"
#include "mapmanipulator.h"

namespace Updraft {
namespace Core {

SceneManager::SceneManager(QString baseEarthFile,
    osgViewer::ViewerBase::ThreadingModel threadingModel) {
  viewer = new osgViewer::Viewer();
  viewer->setThreadingModel(threadingModel);

  // set graphic traits
  osg::GraphicsContext::Traits* traits = createGraphicsTraits(0, 0, 100, 100);
  // create window
  graphicsWindow = createGraphicsWindow(traits);
  // create camera
  camera = createCamera(traits);
  camera->setGraphicsContext(graphicsWindow);
  viewer->setCamera(camera);

  // create map manager
  mapManager = new MapManager(baseEarthFile);

  // create and set scene
  sceneRoot = createInitialScene(mapManager->getMapNode());
  viewer->setSceneData(sceneRoot);

  // set manipulator
  osgEarth::Viewpoint start(14.42046, 50.087811, 0, 0.0, -90.0, /*6e3*/6e7);
  // osgEarth::Util::EarthManipulator* manipulator =
  //   new osgEarth::Util::EarthManipulator();
  MapManipulator* manipulator = new MapManipulator();
  manipulator->setNode(mapManager->getMapNode());
  manipulator->setHomeViewpoint(start);

  viewer->setCameraManipulator(manipulator);
  // or set one specific view:
  // camera->setViewMatrixAsLookAt(osg::Vec3d(0, 0, -6e7),
  //   osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));

  // start drawing
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(redrawScene()));
  timer->start(10);
}

SceneManager::~SceneManager() {
}

QWidget* SceneManager::getWidget() {
  if (graphicsWindow != NULL) {
    return graphicsWindow->getGLWidget();
  } else {
    return NULL;
  }
}

void SceneManager::redrawScene() {
  viewer->frame();
}

osg::GraphicsContext::Traits* SceneManager::createGraphicsTraits
    (int x, int y, int w, int h, const std::string& name,
    bool windowDecoration) {
  osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
  osg::GraphicsContext::Traits* traits = new osg::GraphicsContext::Traits();
  traits->windowName = name;
  traits->windowDecoration = windowDecoration;
  traits->x = x;
  traits->y = y;
  traits->width = w;
  traits->height = h;
  traits->doubleBuffer = true;
  traits->alpha = ds->getMinimumNumAlphaBits();
  traits->stencil = ds->getMinimumNumStencilBits();
  traits->sampleBuffers = ds->getMultiSamples();
  traits->samples = ds->getNumMultiSamples();

  return traits;
}

osgQt::GraphicsWindowQt* SceneManager::createGraphicsWindow
    (osg::GraphicsContext::Traits* traits) {
  return new osgQt::GraphicsWindowQt(traits);
}

osg::Camera* SceneManager::createCamera(osg::GraphicsContext::Traits* traits) {
  osg::Camera* camera = new osg::Camera();

  camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
  camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
  camera->setProjectionMatrixAsPerspective(
    30.0f, static_cast<double>(traits->width)/
    static_cast<double>(traits->height),
    1.0f, 10000.0f);
  return camera;
}

osg::Group* SceneManager::createInitialScene(osgEarth::MapNode* mapNode) {
  osg::Group* root = new osg::Group();

  // add background
  osg::ClearNode* background = new osg::ClearNode();
  background->setClearColor(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
  root->addChild(background);

  // add basic map
  root->addChild(mapNode);
  return root;
}

}  // end namespace Core
}  // end namespace Updraft
