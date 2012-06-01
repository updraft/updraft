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
#include "gimbalcompass.h"

namespace Updraft {
namespace Core {

SceneManager::SceneManager() {
  osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

  manipulator = new MapManipulator();

  viewer = new osgViewer::Viewer();
  viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);

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

  createMapManagers();

  // create and set scene
  sceneRoot = new osg::Group();

  // create and add background
  background = new osg::ClearNode();
  background->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
  sceneRoot->addChild(background);

  // add basic group for nodes
  simpleGroup = new osg::Group();
  sceneRoot->addChild(simpleGroup);

  // add active map
  mapNode = mapManagers[activeMapIndex]->getMapNode();
  // sceneRoot->addChild(mapNode);

  // Make map node pickable
  // registerOsgNode(mapNode, mapManagers[activeMapIndex]->getMapObject());

  viewer->setSceneData(sceneRoot);

  manipulator->setNode(mapNode);
  manipulator->setHomeViewpoint(getInitialPosition());

  viewer->setCameraManipulator(manipulator);

  // Create a picking handler
  viewer->addEventHandler(new PickHandler());
  viewer->addEventHandler(new osgViewer::StatsHandler);

  menuItems();
  mapLayerGroup();

  // Create the gimbal node
  sceneRoot->addChild(new GimbalCompass());

  // start drawing
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(redrawScene()));
  timer->start(20);
}

osgEarth::Util::Viewpoint SceneManager::getHomePosition() {
  return osgEarth::Util::Viewpoint(14.42046, 50.087811,
    0, 0.0, -90.0, 15e5);
}

osgEarth::Util::Viewpoint SceneManager::getInitialPosition() {
  osgEarth::Util::Viewpoint start = getHomePosition();
  start.setRange(start.getRange() * 1.3);
  return start;
}

void SceneManager::menuItems() {
  Menu* viewMenu = updraft->mainWindow->getSystemMenu(MENU_VIEW);

  QAction* resetNorthAction = new QAction(tr("Rotate to north"), this);
  connect(resetNorthAction, SIGNAL(triggered()), this, SLOT(resetNorth()));
  viewMenu->insertAction(200, resetNorthAction);

  QAction* untiltAction = new QAction(tr("Restore 2D View"), this);
  connect(untiltAction, SIGNAL(triggered()), this, SLOT(untilt()));
  viewMenu->insertAction(300, untiltAction);
}

void SceneManager::mapLayerGroup() {
  osgEarth::MapNode* map = getMapNode();
  osg::Group* group = newGroup();
  // TODO(Maria): Get name from the .earth file.
  // QString title = QString::fromStdString(map->getMap()->getName());
  MapLayerGroupInterface* mapLayerGroup =
    updraft->mainWindow->createMapLayerGroup(tr("Maps"), group, map);

  for (int i = 0; i < mapManagers.size(); i++) {
    MapManager *manager = mapManagers[i];

    MapLayerInterface* layer = mapLayerGroup->insertExistingMapLayer(
      manager->getMapNode(), manager->getName());

    layer->connectSignalChecked(
      this, SLOT(checkedMap(bool, MapLayerInterface*)));

    if (i == activeMapIndex) {
      layer->emitDisplayed(true);
    } else {
      layer->emitDisplayed(false);
    }

    layers.append(layer);
  }
}

SceneManager::~SceneManager() {
  // We should unregister all the registered osg nodes
  QList<osg::Node*> registeredNodes = pickingMap.keys();
  foreach(osg::Node* node, registeredNodes) {
    unregisterOsgNode(node);
  }

  foreach(MapManager *m, mapManagers) {
    delete m;
  }
}

QWidget* SceneManager::getWidget() {
  if (graphicsWindow != NULL) {
    return graphicsWindow->getGLWidget();
  } else {
    return NULL;
  }
}

void SceneManager::startInitialAnimation() {
  osgEarth::Util::Viewpoint home = getHomePosition();
  // set home position for ACTION_HOME
  manipulator->setHomeViewpoint(home, 1.0);
  // go to home position now
  manipulator->setViewpoint(home, 2.0);
}

void SceneManager::redrawScene() {
  // start initial animation in second frame to prevent jerks
  static int i = 0;
  if (i == 1) startInitialAnimation();
  if (i < 2) ++i;

  bool isTilted = (manipulator->getViewpoint().getPitch() > -88);
  bool isFar = (manipulator->getDistance() > 7e6);
  if (isTilted || isFar) {
    updateCameraPerspective(camera);
  } else {
    updateCameraOrtho(camera);
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

void SceneManager::checkedMap(bool value, MapLayerInterface* object) {
  // find the map layer:
  int index = 0;
  for (index = 0; index < layers.size(); index++) {
    if (layers[index] == object) break;
  }
  if (index == activeMapIndex) {
      // cannot uncheck the active map
    if (value == false) {
      // force the chcekbox to be visible
      layers[index]->emitDisplayed(true);
    }
  } else {
    // checked non active map
    // replace the map in the scene:
    if (value == true) {
      int oldIndex = activeMapIndex;
      activeMapIndex = index;
      unregisterOsgNode(mapManagers[oldIndex]->getMapNode());
      sceneRoot->removeChild(mapManagers[oldIndex]->getMapNode());
      sceneRoot->addChild(mapManagers[activeMapIndex]->getMapNode());
      registerOsgNode(mapNode, mapManagers[activeMapIndex]->getMapObject());
      layers[oldIndex]->emitDisplayed(false);
      layers[activeMapIndex]->emitDisplayed(true);
    }
  }
}

MapManager* SceneManager::getMapManager() {
  return mapManagers[activeMapIndex];
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
  isCameraPerspective = FALSE;
  updateCameraPerspective(camera);
  return camera;
}

double SceneManager::getAspectRatio() {
  const osg::GraphicsContext::Traits *traits = graphicsWindow->getTraits();
  double aspectRatio = static_cast<double>(traits->width)/
    static_cast<double>(traits->height);
  return aspectRatio;
}

void SceneManager::updateCameraOrtho(osg::Camera* camera) {
  static double lastDistance = 0;
  double distance = manipulator->getDistance();
  bool hasDistanceChanged = (distance != lastDistance);
  // no need to update projection matrix
  if (!hasDistanceChanged && !isCameraPerspective) {
    return;
  }
  lastDistance = distance;

  double hy = distance * 0.2679;  // tan(fovy/2)
  double hx = hy * getAspectRatio();
  camera->setProjectionMatrixAsOrtho2D(-hx, hx, -hy, hy);
  isCameraPerspective = FALSE;
}

void SceneManager::updateCameraPerspective(osg::Camera* camera) {
  // no need to update projection matrix
  if (isCameraPerspective) {
    return;
  }
  camera->setProjectionMatrixAsPerspective(
    30.0f, getAspectRatio(),
    1.0f, 10000.0f);
  isCameraPerspective = TRUE;
}

void SceneManager::resetNorth() {
  manipulator->resetNorth(1);
}

void SceneManager::untilt() {
  manipulator->untilt(1);
}

void SceneManager::createMapManagers() {
  mapManagers.append(
    new MapManager(updraft->getDataDirectory() + "/initial1.earth"));
  mapManagers.append(
    new MapManager(updraft->getDataDirectory() + "/initial2.earth"));

  activeMapIndex = 0;
}

}  // end namespace Core
}  // end namespace Updraft
