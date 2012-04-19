#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>
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

class GraphicsWindow;

/// Wrapper around osgQt::GLWidget to support on demand rendering.
class GraphicsWidget: public osgQt::GLWidget {
 public:
  GraphicsWidget(
    SceneManager* sceneManager,
    QWidget* parent = NULL,
    const QGLWidget* shareWidget = NULL,
    Qt::WindowFlags f = 0,
    bool forwardKeyEvents = false)
    : osgQt::GLWidget(parent, shareWidget, f, forwardKeyEvents),
    sceneManager(sceneManager) {}

  SceneManager* sceneManager;

 protected:
  /// Process event imediately after it is registered.
  bool event(QEvent* ev);
};

/// Wrapper around osgQt::GraphicsWindowQt to support on demand rendering.
class GraphicsWindow: public osgQt::GraphicsWindowQt {
 public:
  explicit GraphicsWindow(SceneManager* sceneManager, osgQt::GLWidget* widget)
    : osgQt::GraphicsWindowQt(widget), sceneManager(sceneManager) {}

  void requestRedraw() {
    qDebug() << "graphics window request redraw";
    sceneManager->requestRedraw = true;
  }

  void requestContinuousUpdate(bool needed) {
    qDebug() << "graphics window request continuous update " << needed;
    sceneManager->requestContinuousUpdate = needed;
  }

 private:
  SceneManager* sceneManager;
};

class Viewer: public osgViewer::Viewer {
 public:
  explicit Viewer(SceneManager* sceneManager)
  : sceneManager(sceneManager) {}

  void requestRedraw() {
    qDebug() << "viewer request redraw";
    sceneManager->requestRedraw = true;
  }

  void requestContinuousUpdate(bool needed) {
    qDebug() << "viewer request continuous update " << needed;
    sceneManager->requestContinuousUpdate = needed;
  }

 private:
  SceneManager* sceneManager;
};

bool GraphicsWidget::event(QEvent* ev) {
  int t = ev->type();
  qDebug() << "event detected " << t;
  sceneManager->eventDetected = true;
  return osgQt::GLWidget::event(ev);
}

SceneManager::SceneManager(QString baseEarthFile)
  : requestRedraw(false), requestContinuousUpdate(false), eventDetected(false) {
  osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

  viewer = new Viewer(this);
  viewer->setThreadingModel(osgViewer::ViewerBase::ThreadPerContext);

  GraphicsWidget *widget = new GraphicsWidget(this);
  graphicsWindow = new GraphicsWindow(this, widget);
  widget->setMouseTracking(false);

  // create camera
  camera = createCamera();
  camera->setGraphicsContext(graphicsWindow);
  camera->setClearStencil(128);
  camera->setClearMask
    (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  viewer->setCamera(camera);

  is2dEnabled = 0;
  setPerspectiveCamera(camera);

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
  MapManipulator* manipulator = new MapManipulator();
  manipulator->setNode(mapManager->getMapNode());
  manipulator->setHomeViewpoint(start);

  viewer->setCameraManipulator(manipulator);
  // or set one specific view:
  // camera->setViewMatrixAsLookAt(osg::Vec3d(0, 0, -6e7),
    // osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));

  // Create a picking handler
  viewer->addEventHandler(new PickHandler());

  // insert menu item for switching 2d/3d
  QAction* toggleViewAction = new QAction("Toggle 2D/3D view", this);
  connect(toggleViewAction, SIGNAL(triggered()), this, SLOT(toggleView()));
  Menu* toolsMenu = updraft->mainWindow->getSystemMenu(MENU_TOOLS);
  toolsMenu->insertAction(1, toggleViewAction);

  // start drawing
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
  timer->start(20);
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

void SceneManager::tick() {
  bool needRedraw = requestRedraw || requestContinuousUpdate ||
    viewer->getDatabasePager()->requiresUpdateSceneGraph() ||
    viewer->getDatabasePager()->getRequestsInProgress() ||
    viewer->getCamera()->getUpdateCallback() ||
    viewer->getSceneData()->getNumChildrenRequiringUpdateTraversal() > 0;

  if (eventDetected && !needRedraw) {
    qDebug() << "event processing";
    viewer->eventTraversal();
    needRedraw = requestRedraw || requestContinuousUpdate;
  }

  if (needRedraw) {
    qDebug() << "frame";
    redrawScene();
  }

  requestRedraw = false;
  eventDetected = false;
}

void SceneManager::redrawScene() {
  if (is2dEnabled) {
    updateOrthographicCamera(camera);
  }
  viewer->frame();
}

void SceneManager::toggleView() {
  is2dEnabled = !is2dEnabled;

  if (is2dEnabled) {
    // TODO(Bohdan): untilt before switching to orthographic view
    setOrthographicCamera(camera);
    qDebug("Switched to 2D");
  } else {
    setPerspectiveCamera(camera);
    qDebug("Switched to 3D");
  }
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

osg::Camera* SceneManager::createCamera() {
  const osg::GraphicsContext::Traits* traits = graphicsWindow->getTraits();
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
  osg::Vec3d eye, center, up;
  camera->getViewMatrixAsLookAt(eye, center, up);

  double distance =  eye.length() - 6365000.f;
  double fovy = 30.f;
  double hy = distance * fovy/90.0f;
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
