#include "plugin.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include <osgEarthUtil/ObjectPlacer>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>

#include "../../coreinterface.h"
#include "../../core/ui/maplayergroup.h"
#include "../../maplayerinterface.h"

namespace Updraft {

TestPlugin::TestPlugin() {}

TestPlugin::~TestPlugin() {
  delete mapLayerGroup;
}

QString TestPlugin::getName() {
  return "testplugin";
}

unsigned TestPlugin::getPriority() {
  return 1;
}

void TestPlugin::initialize() {
  // Help about this plugin
  helpAction = new QAction("About testplugin...", this);
  connect(helpAction, SIGNAL(triggered()), this, SLOT(showHelp()));

  helpAction2 = new QAction("About testplugin (higher priority)...", this);
  connect(helpAction2, SIGNAL(triggered()), this, SLOT(showHelp()));

  core->getSystemMenu(MENU_HELP)->insertAction(1, helpAction);
  core->getSystemMenu(MENU_HELP)->insertAction(0, helpAction2);

  // Create a new menu and insert a test action into it as well
  myMenu = core->createMenu("Test plugin");

  helpAction3 = new QAction("About testplugin...", this);
  connect(helpAction3, SIGNAL(triggered()), this, SLOT(showHelp()));

  myMenu->insertAction(0, helpAction3);

  // Add an action into a context menu
  core->getSystemMenu(MENU_CONTEXT)->insertAction(0, helpAction);

  createTab("Button tab 1");
  createTab("Button tab 2");

  FileRegistration regTxt1;
  regTxt1.category = CATEGORY_TEMPORARY;
  regTxt1.extension = ".txt";
  regTxt1.typeDescription = "Just a text file";
  regTxt1.roleDescription = "Open text file for reading";
  regTxt1.roleId = 1;
  regTxt1.plugin = this;

  core->registerFiletype(regTxt1);

  FileRegistration regTxt2;
  regTxt2.category = CATEGORY_PERSISTENT;
  regTxt2.extension = ".txt";
  regTxt2.typeDescription = "Labels file";
  regTxt2.roleDescription = "Import city names";
  regTxt2.importDirectory = "testTxt";
  regTxt2.roleId = 2;
  regTxt2.plugin = this;

  core->registerFiletype(regTxt2);

  FileRegistration regTxt3;
  regTxt3.category = CATEGORY_PERSISTENT;
  regTxt3.extension = ".txt";
  regTxt3.typeDescription = "Secret file";
  regTxt3.roleDescription = "Import something important";
  regTxt3.importDirectory = "testImportantTxt";
  regTxt3.roleId = 3;
  regTxt3.plugin = this;

  core->registerFiletype(regTxt3);

  FileRegistration regPng;
  regPng.category = CATEGORY_PERSISTENT;
  regPng.extension = ".png";
  regPng.typeDescription = "Png image. Maybe a map?";
  regPng.roleDescription = "Import map layer";
  regPng.importDirectory = "testPngFiles";
  regPng.roleId = 4;
  regPng.plugin = this;

  core->registerFiletype(regPng);

  FileRegistration regHtml;
  regHtml.category = CATEGORY_TEMPORARY;
  regHtml.extension = ".png";
  regHtml.typeDescription = "bzzzz";
  regHtml.roleDescription = "Import it";
  regHtml.roleId = 5;
  regHtml.plugin = this;

  core->registerFiletype(regHtml);

  // Create map layers items in the left pane.
  mapLayerGroup = core->createMapLayerGroup("Test group");
  if (mapLayerGroup != NULL) {
    // Connect display/hide signals with slots.

    // create map placer: to draw in the map
    osgEarth::MapNode* mapNode = mapLayerGroup->getMapNode();
    osgEarth::Util::ObjectPlacer* objectPlacer =
      new osgEarth::Util::ObjectPlacer(mapNode, 0, false);

    // draw some lines in Czech Republic
    osg::Geode* geode = new osg::Geode();
    osg::Geometry* geom = new osg::Geometry();
    geode->addDrawable(geom);
    geom->setUseDisplayList(false);
    osg::DrawArrays* drawArrayLines = new
      osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
    geom->addPrimitiveSet(drawArrayLines);
    osg::Vec3Array* vertexData = new osg::Vec3Array();
    geom->setVertexArray(vertexData);

    vertexData->push_back(osg::Vec3(0, 0, 0));
    vertexData->push_back(osg::Vec3(0, 0, 2000));
    vertexData->push_back(osg::Vec3(100000, 0, 4000));
    vertexData->push_back(osg::Vec3(100000, -100000, 4000));
    vertexData->push_back(osg::Vec3(0, 0, 0));

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));  // index 0 red
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));  // index 1 green
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));  // index 2 blue
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));  // index 3 white
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));  // index 4 red

    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    drawArrayLines->setFirst(0);
    drawArrayLines->setCount(vertexData->size());

    // change the thickness of the line
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(2000.0f);

    geode->getOrCreateStateSet()->setAttributeAndModes(linewidth,
      osg::StateAttribute::ON);

    osg::Node* randomLines = objectPlacer->placeNode(geode,
      50.087811, 14.42046, 1000);

    Updraft::MapLayerInterface* layer1 =
      mapLayerGroup->insertMapLayer(randomLines, "Relative Lines", 0);
    mapLayers.insert(layer1->id, layer1);
    layer1->connectSignalDisplayed(this, SLOT(mapLayerDisplayed(int, bool)));

    // mapLayerGroup->insertMapLayer(0, layer1, "Relative Lines");

    // draw route from Brno to Plzen:
    osg::Geode* BrnoPlzen = new osg::Geode();
    osg::Geometry* geom2 = new osg::Geometry();
    BrnoPlzen->addDrawable(geom2);
    geom2->setUseDisplayList(false);
    osg::DrawArrays* drawArrayLines2 = new
      osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
    geom2->addPrimitiveSet(drawArrayLines2);
    osg::Vec3Array* vertexData2 = new osg::Vec3Array();
    geom2->setVertexArray(vertexData2);

    // placer
    osg::Matrixd brnoTransformation;
    osg::Matrixd plzenTransformation;

    objectPlacer->createPlacerMatrix(49.11, 16.37, 6000, brnoTransformation);
    objectPlacer->createPlacerMatrix(49.44, 13.22, 600, plzenTransformation);

    osg::Vec3 brno = osg::Vec3(0, 0, 0) * brnoTransformation;
    osg::Vec3 plzen = osg::Vec3(0, 0, 0) * plzenTransformation;

    vertexData2->push_back(brno);
    vertexData2->push_back(plzen);

    drawArrayLines2->setFirst(0);
    drawArrayLines2->setCount(vertexData2->size());

    // change the thickness of the line
    osg::LineWidth* linewidth2 = new osg::LineWidth();
    linewidth2->setWidth(20000.0f);
    BrnoPlzen->getOrCreateStateSet()->setAttributeAndModes(linewidth2,
      osg::StateAttribute::ON);

    // change the color of the line:
    osg::Vec4Array* colors2 = new osg::Vec4Array();
    colors2->push_back(osg::Vec4(1, 1, 0, 1));  // yellow!

    geom2->setColorArray(colors2);
    geom2->setColorBinding(osg::Geometry::BIND_OVERALL);

    /*
    Layer l2;
    l2.osgNode = BrnoPlzen;
    MapLayer* layer2 = new MapLayer(MapLayerType::OSG_NODE_LAYER, l2);

    mapLayerGroup->insertMapLayer(1, layer2, "Brno to Plzen");
    */
  }

  qDebug("testplugin loaded");
}

void TestPlugin::createTab(QString title) {
  QWidget* container = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(container);

  QPushButton* btn1 = new QPushButton("Show help", container);
  QPushButton* btn2 = new QPushButton("Close this tab", container);

  layout->addWidget(btn1);
  layout->addWidget(btn2);

  TabInterface* tab = core->createTab(container, title);

  connect(btn1, SIGNAL(clicked()), this, SLOT(showHelp()));
  // connect(btn2, SIGNAL(clicked()), tab, SLOT(showHelp()));
  tab->connectSlotClose(btn2, SIGNAL(clicked()));
}

void TestPlugin::deinitialize() {
  qDebug("testplugin unloaded");
}

void TestPlugin::showHelp() {
  QMainWindow* win = core->getMainWindow();

  QMessageBox::information(win, "About testplugin",
    "Testplugin is just a test plugin to see whether our api is working");
}

bool TestPlugin::fileOpen(const QString &filename, int roleId) {
  switch (roleId) {
    case 1:
    case 2:
    case 3:
      qDebug() << "Testplugin opens file " << filename << ", role " << roleId;
      break;

    default:
      qDebug() << "Testplugin: Unknown file role.";
      return false;
  }

  return true;
}

void TestPlugin::fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename) {
  if (roles != NULL) {
    if (filename.endsWith("txt")) {
      roles->append("Txt file, role 1");
      roles->append("Txt file, role 2");
      roles->append("Txt file, role 3");
    } else {
      roles->append("other file, the only role");
    }
  }
}

void TestPlugin::mapLayerDisplayed(int id, bool value) {
  mapLayers[id]->setVisible(value);
}

Q_EXPORT_PLUGIN2(testplugin, TestPlugin)

}  // End namespace Updraft

