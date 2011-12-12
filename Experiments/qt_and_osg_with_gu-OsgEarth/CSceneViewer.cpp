#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarth/ImageLayer>
#include <osgEarth/ViewPoint>
#include <osgEarthUtil/ObjectPlacer>
#include <osgEarthUtil/EarthManipulator>
#include <osg/PositionAttitudeTransform>
#include <QDebug>

#include "CSceneViewer.h"


CSceneViewer::CSceneViewer(osgViewer::ViewerBase::ThreadingModel threadingModel/*, osg::Camera* camera, osg::Node* scene*/)
   : osgViewer::Viewer()
{
   setThreadingModel(threadingModel);
   //setCamera( camera );

   osg::Camera* camera = createCamera(0,0,100,100);
   setCamera( camera );
   osg::Group* root = new osg::Group();

   osg::Node* scene = osgDB::readNodeFile("C:/data/arc_imagery-elevation.earth");
   root->addChild(scene);

   osg::Node * planeNode=osgDB::readNodeFile("c:/data/cessna.osgt");
   osg::Node * planeNode2=osgDB::readNodeFile("c:/data/cessna.osgt");

	// find the mapNode
   mapNode = osgEarth::MapNode::findMapNode (scene);

   osgEarth::Util::ObjectPlacer* objectPlacer = new osgEarth::Util::ObjectPlacer(mapNode,0,false); 

   //osgEarth::ImageLayer* baseLayer = mapNode->getMap()->getImageLayerByName("base");
   
   
	osg::Vec3 scale(100000, 100000, 100000);
	osg::Vec3 scale2(10, 10, 10);

   osg::PositionAttitudeTransform* planeScale = new osg::PositionAttitudeTransform ();
   planeScale->addChild(planeNode2);
   planeScale->setScale(scale2);
   osg::Node * placerNode = objectPlacer->placeNode(planeScale, 50.087811, 14.42046,400); 

   osg::ClearNode* background = new osg::ClearNode();
   background->setClearColor(osg::Vec4(0.8f,0.6f,0.4f,1.0f));
   root->addChild(background);
   root->addChild(placerNode);

   osg::PositionAttitudeTransform* planeTransform = new osg::PositionAttitudeTransform ();
   planeTransform->addChild(planeNode);
   //root->addChild(planeTransform);
   osg::Vec3 pyramidTwoPosition(0,-30000000,0);
	planeTransform->setPosition( pyramidTwoPosition );
	planeTransform->setScale(scale);

   //root->removeChild(scene);
   //root->addChild(planeNode2);

   //if (baseLayer == NULL) qDebug<<"neni";
		// DELETE THIS LAYER
   //mapNode->getMap()->removeImageLayer(baseLayer);

		// ADD LAYER:
   /*osgEarth::Drivers::TMSOptions driverOpt;
	driverOpt.url() = "http://tile.openstreetmap.org/";
	driverOpt.tmsType() = "google";

	osgEarth::ImageLayerOptions layerOpt( "OSM", driverOpt );
	layerOpt.profile() = osgEarth::ProfileOptions( "global-mercator" );

	osgEarth::ImageLayer* osmLayer = new osgEarth::ImageLayer( "streetmap", layerOpt );
	mapNode->getMap()->addImageLayer( osmLayer );*/

	osg::Vec3d eye;
	osg::Vec3d dir;
	osg::Vec3d up;

   setSceneData( root );


//   osgEarth::Viewpoint* start = new osgEarth::Viewpoint(14.42046, 50.087811, 100000, 0.0, -90.0, 6e6);
   osgEarth::Viewpoint start (14.42046, 50.087811, 0, 0.0, -90.0, 6e3);
   osgEarth::Util::EarthManipulator* manipulator = new osgEarth::Util::EarthManipulator();
   manipulator->setNode(mapNode);
   manipulator->setHomeViewpoint(start);

	setCameraManipulator(manipulator);
   //setCameraManipulator( new osgGA::TrackballManipulator );

   camera->getViewMatrixAsLookAt(eye, dir, up);
   qDebug() << eye.x() << eye.y() << eye.z() << endl;
   qDebug() << dir.x() << dir.y() << dir.z() << endl;
   qDebug() << up.x() << up.y() << up.z() << endl;
   //qDebug() << getCamera()->getProjectionMatrix()->getTra
}

CSceneViewer::~CSceneViewer()
{
}

QWidget* CSceneViewer::getWidget()
{
   osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( getCamera()->getGraphicsContext() );
   return gw ? gw->getGLWidget() : NULL;
}

void CSceneViewer::redrawScene()
{
	/*osg::Vec3d eye;
	osg::Vec3d dir;
	osg::Vec3d up;

	   getCamera()->getViewMatrixAsLookAt(eye, dir, up);
   qDebug() << eye.x() << eye.y() << eye.z() << endl;
   qDebug() << dir.x() << dir.y() << dir.z() << endl;
   qDebug() << up.x() << up.y() << up.z() << endl;*/
   frame();
}

osg::Camera* CSceneViewer::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration )
{
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