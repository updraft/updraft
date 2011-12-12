#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgDB/ReadFile>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarth/ImageLayer>
#include <osgEarth/ViewPoint>
#include <osgEarthUtil/ObjectPlacer>
#include <osgEarthUtil/EarthManipulator>
#include <osg/PositionAttitudeTransform>
#include <QDebug>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/arcgis/ArcGISMapLayerFactory>

#include "SceneViewer.h"
#include "MapManipulator.h"

SceneViewer::SceneViewer(osgViewer::ViewerBase::ThreadingModel threadingModel)
  : osgViewer::Viewer() {
  setThreadingModel(threadingModel);

  // create camera
  camera = createCamera(0,0,100,100);
  setCamera( camera );

  // create and set scene
  osg::Group* root = createScene ();
  setSceneData( root );

  // set manipulator
  osgEarth::Viewpoint start (14.42046, 50.087811, 0, 0.0, -90.0, /*6e3*/ 6e7);
  //osgEarth::Util::EarthManipulator* manipulator = new osgEarth::Util::EarthManipulator();
  MapManipulator* manipulator = new MapManipulator ();
  manipulator->setNode(mapNode);
  manipulator->setHomeViewpoint(start);

  setCameraManipulator(manipulator);
  // or set one specific view:
  //camera->setViewMatrixAsLookAt(osg::Vec3d(0, 0, -6e7), osg::Vec3d(0, 0, 0), osg::Vec3d(0, 1, 0));
}

SceneViewer::~SceneViewer() {
}

QWidget* SceneViewer::getWidget() {
  osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( getCamera()->getGraphicsContext() );
  return gw ? gw->getGLWidget() : NULL;
}

void SceneViewer::redrawScene() {
  frame();
}

osg::Camera* SceneViewer::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration) {
  osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
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

  osg::ref_ptr<osg::Camera> camera = new osg::Camera;
  camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );

  camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
  camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
  camera->setProjectionMatrixAsPerspective(
    30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
  camera->setViewMatrixAsLookAt(osg::Vec3d(0, -64200000, 0), osg::Vec3d(0, 1, 0), osg::Vec3d(0, 0, 1));
  return camera.release();
}

osg::Group* SceneViewer::createScene () {

  osg::Group* root = new osg::Group();

  osg::ClearNode* background = new osg::ClearNode();
  background->setClearColor(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
  root->addChild(background);

  osg::Node* map = createMap(); 
  root->addChild (map);
  mapNode = osgEarth::MapNode::findMapNode (map);

  return root;
}

osgEarth::MapNode* SceneViewer::createMap (){
  osgEarth::Map* map = new osgEarth::Map();
  map->setName("globe");

  // Add an image layer for offline:
  osgEarth::Drivers::GDALOptions gdal;
  gdal.url() = "C:/Documents and Settings/Mcc/My Documents/Mff Uk/SW Projekt/GIT/updraft/Updraft/data/world.tif";
  map->addImageLayer (new osgEarth::ImageLayer("offline-pic", gdal));

  // Add a terrain map layer
  osgEarth::Drivers::ArcGISOptions arc;
  arc.url() = "http://server.arcgisonline.com/ArcGIS/rest/services/ESRI_Imagery_World_2D/MapServer";
  map->addImageLayer (new osgEarth::ImageLayer( "online-map", arc));

  // Add an elevation layer 
  osgEarth::Drivers::TMSOptions tms;
  tms.url() = "http://readymap.org/readymap/tiles/1.0.0/9/";
  map->addElevationLayer (new osgEarth::ElevationLayer( "online-elevation", tms));

  // Create a MapNode to render this map:
  return new osgEarth::MapNode( map ); 
}