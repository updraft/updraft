#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>
#include <QtGui/QGridLayout>

#include "CSceneViewer.h"
#include "MainWindow.h"


	//Set correct path to model!
const char PATH[] = "C:/data/cessna.osgt";

	// the path to the OSGEarth earth file
const char EARTH_FILE[] = "C:/data/arc_imagery-elevation.earth";

CMainWindow::CMainWindow(osgViewer::ViewerBase::ThreadingModel threadingModel)
   : QMainWindow(), sceneViewer(NULL)
{
   ui.setupUi(this);

   sceneViewer = new CSceneViewer(threadingModel, createCamera(0,0,100,100), osgDB::readNodeFile(EARTH_FILE));

   QWidget *widget = sceneViewer->getWidget();
   ui.gridLayout_2->addWidget( widget );

   timer = new QTimer ();
   connect( timer, SIGNAL(timeout()), this, SLOT(doUpdate()));
   timer->start(10);
}

CMainWindow::~CMainWindow()
{

}

void CMainWindow::doUpdate() 
{
	update();
}

osg::Camera* CMainWindow::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration )
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

void CMainWindow::paintEvent( QPaintEvent* event )
{
   sceneViewer->redrawScene();
}