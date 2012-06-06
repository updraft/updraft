#include <osgQt/GraphicsWindowQt>
#include <osg/CoordinateSystemNode>
#include <osg/Vec3d>
#include <osgEarth/Viewpoint>
#include <osgEarthUtil/ObjectPlacer>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthUtil/ElevationManager>
#include <string>

#include "scenemanager.h"
#include "mapmanipulator.h"
#include "pickhandler.h"

#include "updraft.h"
#include "settingsmanager.h"
#include "ui/mainwindow.h"
#include "ui/menu.h"
#include "../menuinterface.h"

namespace Updraft {
namespace Core {

SceneManager::SceneManager() {
  // Create a group for map settings
  updraft->settingsManager->addGroup(
    "map", tr("Map options"), GROUP_ADVANCED, ":/core/icons/map.png");

  osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

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
  elevationManager = createElevationManager();

  // create and set scene
  sceneRoot = new osg::Group();

  // create and add background
  background = new osg::ClearNode();
  background->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
  sceneRoot->addChild(background);

  // add basic group for nodes
  simpleGroup = updraft->mainWindow->getInvisibleRootMapLayerGroup()->
    getNodeGroup();
  sceneRoot->addChild(simpleGroup);

  // add active map
  mapNode = mapManagers[activeMapIndex]->getMapNode();
  sceneRoot->addChild(mapNode);

  // Make map node pickable
  registerOsgNode(mapNode, mapManagers[activeMapIndex]->getMapObject());

  viewer->setSceneData(sceneRoot);

  manipulator = mapManagers[activeMapIndex]->getManipulator();
  manipulator->setHomeViewpoint(getInitialPosition());

  viewer->setCameraManipulator(manipulator);

  // Create a picking handler
  viewer->addEventHandler(new PickHandler());
  viewer->addEventHandler(new osgViewer::StatsHandler);

  menuItems();
  mapLayerGroup();

  placer = new osgEarth::Util::ObjectPlacer(mapNode, 0, false);

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
  MapLayerGroupInterface* mapLayerGroup = updraft->mainWindow->
    getInvisibleRootMapLayerGroup()->createMapLayerGroup(tr("Maps"));
  mapLayerGroup->setCheckable(false);

  for (int i = 0; i < mapManagers.size(); i++) {
    MapManager *manager = mapManagers[i];

    MapLayerInterface* layer = mapLayerGroup->createMapLayerNoInsert(
      manager->getMapNode(), manager->getName());

    layer->connectSignalChecked(
      this, SLOT(checkedMap(bool, MapLayerInterface*)));

    layer->setChecked(i == activeMapIndex);

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

  delete placer;
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

const osg::EllipsoidModel* SceneManager::getCurrentMapEllipsoid() {
  return mapManagers[activeMapIndex]->getManipulator()->
    getSRS()->getEllipsoid();
}

void SceneManager::redrawScene() {
  // start initial animation in second frame to prevent jerks
  static int i = 0;
  if (i == 1) startInitialAnimation();
  if (i < 2) ++i;

  getMapManager()->getManipulator()->updateCameraProjection();

  viewer->frame();
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

osgEarth::Util::ElevationManager* SceneManager::getElevationManager() {
  return elevationManager;
}

void SceneManager::registerOsgNode(osg::Node* node, MapObject* mapObject) {
  pickingMap.insert(node, mapObject);
}

void SceneManager::unregisterOsgNode(osg::Node* node) {
  pickingMap.remove(node);
}

osgEarth::Util::ElevationManager* SceneManager::createElevationManager() {
  for (int i = 0; i < mapManagers.size(); i++) {
    if (mapManagers[i]->hasElevation()) {
      return (new osgEarth::Util::ElevationManager(
        mapManagers[i]->getMap()));
    }
  }
    // if no map had the elevation specified
    // create a dummy elevation manager
  return (new osgEarth::Util::ElevationManager(
    mapManagers[activeMapIndex]->getMap()));
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
      layers[index]->setChecked(true);
    }
  } else {
    // checked non active map
    // replace the map in the scene:
    if (value == true) {
      osgEarth::Util::EarthManipulator::CameraProjection projection;
      projection = osgEarth::Util::EarthManipulator::PROJ_PERSPECTIVE;
      manipulator->getSettings()->setCameraProjection(projection);
      viewer->frame();

      int oldIndex = activeMapIndex;
      activeMapIndex = index;
      unregisterOsgNode(mapManagers[oldIndex]->getMapNode());
      sceneRoot->removeChild(mapManagers[oldIndex]->getMapNode());

        // set current map node
      mapNode = mapManagers[activeMapIndex]->getMapNode();
      sceneRoot->addChild(mapNode);

      osgEarth::Util::Viewpoint viewpoint = manipulator->getViewpoint();

        // if the active map has a terrain, check whether we're not under
      if (mapManagers[activeMapIndex]->hasElevation()) {
        double elevation, resolution;
        if (elevationManager->getElevation(viewpoint.x(), viewpoint.y(),
          0, 0, elevation, resolution)) {
          if (viewpoint.getRange() < elevation) {
            viewpoint.setRange(elevation + 10);
          }
        }
      }

      // manipulator = mapManagers[activeMapIndex]->getManipulator();
      manipulator = new MapManipulator();
      manipulator->setHomeViewpoint(viewpoint);
      mapManagers[activeMapIndex]->setManipulator(manipulator);

      viewer->setCameraManipulator(manipulator);

      registerOsgNode(mapNode, mapManagers[activeMapIndex]->getMapObject());
      layers[oldIndex]->setChecked(false);
      layers[activeMapIndex]->setChecked(true);
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

  camera->setProjectionMatrixAsPerspective(
    30.0f, getAspectRatio(),
    1.0f, 10000.0f);

  return camera;
}

double SceneManager::getAspectRatio() {
  const osg::GraphicsContext::Traits *traits = graphicsWindow->getTraits();
  double aspectRatio = static_cast<double>(traits->width)/
    static_cast<double>(traits->height);
  return aspectRatio;
}

void SceneManager::resetNorth() {
  manipulator->resetNorth(1);
}

void SceneManager::untilt() {
  manipulator->untilt(1);
}

void SceneManager::createMapManagers() {
  QDir dataDir = updraft->getStaticDataDirectory();
  mapManagers.append(
    new MapManager(dataDir.absoluteFilePath("initial1.earth"),
      tr("OpenStreetMaps")));
  mapManagers.append(
    new MapManager(dataDir.absoluteFilePath("initial2.earth"),
      tr("ArcGIS, Satellite Imagery")));
  mapManagers.append(
    new MapManager(dataDir.absoluteFilePath("initial3.earth"),
      tr("ArcGIS, Topographic Map")));

  activeMapIndex = 0;
}

osgEarth::Util::ObjectPlacer* SceneManager::getObjectPlacer() {
  return placer;
}

}  // end namespace Core
}  // end namespace Updraft
