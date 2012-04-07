#include <osgQt/GraphicsWindowQt>
#include <osgEarthUtil/Viewpoint>
#include <osgEarthUtil/ObjectPlacer>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <string>

#include "scenemanager.h"
#include "mapmanipulator.h"
#include "pickhandler.h"

#include "updraft.h"
#include "ui/mainwindow.h"
#include "ui/menu.h"
#include "../menuinterface.h"

namespace Updraft {
namespace Core {

SceneManager::SceneManager(QString baseEarthFile,
    osgViewer::ViewerBase::ThreadingModel threadingModel) {
  osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

  manipulator = new MapManipulator();

  viewer = new osgViewer::Viewer();
  viewer->setThreadingModel(threadingModel);

  // set graphic traits
  osg::GraphicsContext::Traits* traits = createGraphicsTraits(0, 0, 100, 100);
  // create window
  graphicsWindow = createGraphicsWindow(traits);
  // create camera
  camera = createCamera(traits);
  camera->setGraphicsContext(graphicsWindow);
  camera->setClearStencil(128);
  camera->setClearMask
    (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  viewer->setCamera(camera);

  // create map manager
  mapManager = new MapManager(baseEarthFile);

  // create and set scene
  sceneRoot = new osg::Group();
    // create and add background
  background = new osg::ClearNode();
  background->setClearColor(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
  sceneRoot->addChild(background);
    // add basic group for nodes
  simpleGroup = new osg::Group();
  sceneRoot->addChild(simpleGroup);
    // add basic map
  mapNode = mapManager->getMapNode();
  sceneRoot->addChild(mapNode);

  viewer->setSceneData(sceneRoot);

  // set manipulator
  osgEarth::Util::Viewpoint start(14.42046, 50.087811,
    0, 0.0, -90.0, 12e6 /*6e7*/);
  // osgEarth::Util::EarthManipulator* manipulator =
    // new osgEarth::Util::EarthManipulator();

  manipulator->setNode(mapManager->getMapNode());
  manipulator->setHomeViewpoint(start);

  viewer->setCameraManipulator(manipulator);

  // or set one specific view:
  // camera->setViewMatrixAsLookAt(osg::Vec3d(0, 0, -6e7),
    // osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));

  // Create a picking handler
  viewer->addEventHandler(new PickHandler());

  // start drawing
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(redrawScene()));
  timer->start(10);
}

SceneManager::~SceneManager() {
  // We should unregister all the registered osg nodes
  QList<osg::Node*> registeredNodes = pickingMap.keys();
  foreach(osg::Node* node, registeredNodes) {
    unregisterOsgNode(node);
  }
}

QWidget* SceneManager::getWidget() {
  if (graphicsWindow != NULL) {
    return graphicsWindow->getGLWidget();
  } else {
    return NULL;
  }
}

void SceneManager::redrawScene() {
  osgEarth::Util::Viewpoint viewpoint = manipulator->getViewpoint();
  bool isTilted = (viewpoint.getPitch() > -88);
  bool isFar = (manipulator->getDistance() > 7e6);
  if (isTilted || isFar) {
    setPerspectiveCamera(camera);
  } else {
    updateOrthographicCamera(camera);
  }
  viewer->frame();
}

osg::Group* SceneManager::newGroup() {
  osg::Group* group = new osg::Group();
  sceneRoot->addChild(group);
  return group;
}

bool SceneManager::removeGroup(osg::Group* group) {
  return sceneRoot->removeChild(group);
}

osgEarth::MapNode* SceneManager::getMapNode() {
  return mapNode;
}

osg::Group* SceneManager::getSimpleGroup() {
  return simpleGroup;
}

void SceneManager::registerOsgNode(osg::Node* node, MapObject* mapObject) {
  pickingMap.insert(node, mapObject);
}

void SceneManager::unregisterOsgNode(osg::Node* node) {
  pickingMap.remove(node);
}

MapObject* SceneManager::getNodeMapObject(osg::Node* node) {
  QHash<osg::Node*, MapObject*>::iterator it = pickingMap.find(node);
  if (it != pickingMap.end()) {
    return it.value();
  } else {
    return NULL;
  }
}

MapManager* SceneManager::getMapManager() {
  return mapManager;
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
  setOrthographicCamera(camera);
  return camera;
}

double SceneManager::getAspectRatio() {
  const osg::GraphicsContext::Traits *traits = graphicsWindow->getTraits();
  double aspectRatio = static_cast<double>(traits->width)/
    static_cast<double>(traits->height);
  return aspectRatio;
}

void SceneManager::updateOrthographicCamera(osg::Camera* camera) {
  double distance = manipulator->getDistance();
  double hy = distance * 0.2679;  // tan(fovy/2)
  double hx = hy * getAspectRatio();
  camera->setProjectionMatrixAsOrtho2D(-hx, hx, -hy, hy);
}

void SceneManager::setOrthographicCamera(osg::Camera* camera) {
  updateOrthographicCamera(camera);
}

void SceneManager::setPerspectiveCamera(osg::Camera* camera) {
  camera->setProjectionMatrixAsPerspective(
    30.0f, getAspectRatio(),
    1.0f, 10000.0f);
}

}  // end namespace Core
}  // end namespace Updraft
