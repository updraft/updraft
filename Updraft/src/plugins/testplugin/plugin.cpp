#include "plugin.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include <osgEarthUtil/ObjectPlacer>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>

#include "../../coreinterface.h"
#include "../../maplayergroupinterface.h"
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

  // createTab("Button tab 1");
  // createTab("Button tab 2");

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
  // osg::Group* simpleGroup = core->getSimpleGroup();

  // Create map layers items in the left pane.
  mapLayerGroup = core->createMapLayerGroup("Test group");
  if (mapLayerGroup != NULL) {
    // create map placer: to draw in the map
    osgEarth::Util::ObjectPlacer* objectPlacer =
      mapLayerGroup->getObjectPlacer();

    // LAYER #1
    // draw some lines in Czech Republic
    osg::Geode* geode = new osg::Geode();  // the root of the subscene
    osg::Geometry* geom = new osg::Geometry();
    geode->addDrawable(geom);
    geom->setUseDisplayList(false);
    // we will be drawing lines
    osg::DrawArrays* drawArrayLines = new
      osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
    geom->addPrimitiveSet(drawArrayLines);
    // create vertex array for the lines
    osg::Vec3Array* vertexData = new osg::Vec3Array();
    geom->setVertexArray(vertexData);

    // add vertices into the vertex array
    vertexData->push_back(osg::Vec3(0, 0, 0));
    vertexData->push_back(osg::Vec3(0, 0, 2000));
    vertexData->push_back(osg::Vec3(100000, 0, 4000));
    vertexData->push_back(osg::Vec3(100000, -100000, 4000));
    vertexData->push_back(osg::Vec3(0, 0, 0));

    // set color array: one color for each vertex
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));  // index 0 red
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));  // index 1 green
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));  // index 2 blue
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));  // index 3 white
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));  // index 4 red

    geom->setColorArray(colors);
    // set the colors to be one color per one vertex
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    // set where to start the line strip and how long it will be
    drawArrayLines->setFirst(0);
    drawArrayLines->setCount(vertexData->size());

    // create the thickness of the line object
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(3.0f);

    // set the thickness to the scene root
    geode->getOrCreateStateSet()->setAttributeAndModes(linewidth,
      osg::StateAttribute::ON);

    // move the center of the node to Prague
    /*osg::Node* randomLines = objectPlacer->placeNode(geode,
      50.087811, 14.42046, 1000);
    
    Updraft::MapLayerInterface* layer1 =
      mapLayerGroup->insertMapLayer(randomLines, "Relative Lines", 0);
    layer1->connectSignalChecked
      (this, SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
    mapLayers.append(layer1);
    */

    // LAYER #2
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

    osg::Matrixd brnoTransformation;
    osg::Matrixd plzenTransformation;

    objectPlacer->createPlacerMatrix(49, 16, 6000, brnoTransformation);
    objectPlacer->createPlacerMatrix(50, 13, 6000, plzenTransformation);

    // place the points according to matrices
    osg::Vec3 brno = osg::Vec3(0, 0, 0) * brnoTransformation;
    osg::Vec3 plzen = osg::Vec3(0, 0, 0) * plzenTransformation;

    vertexData2->push_back(brno);
    vertexData2->push_back(plzen);

    drawArrayLines2->setFirst(0);
    drawArrayLines2->setCount(vertexData2->size());

    // change the thickness of the line
    osg::LineWidth* linewidth2 = new osg::LineWidth();
    linewidth2->setWidth(3.0f);
    BrnoPlzen->getOrCreateStateSet()->setAttributeAndModes(linewidth2,
      osg::StateAttribute::ON);

    // change the color of the line:
    osg::Vec4Array* colors2 = new osg::Vec4Array();
    colors2->push_back(osg::Vec4(1, 1, 0, 1));  // yellow!

    geom2->setColorArray(colors2);
    geom2->setColorBinding(osg::Geometry::BIND_OVERALL);

    /*
    MapLayerInterface* layer2 =
      mapLayerGroup->insertMapLayer(BrnoPlzen, "Brno to Plzen");

    // simpleGroup->addChild(BrnoPlzen);
    layer2->connectSignalChecked
      (this, SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
    mapLayers.append(layer2);
    */

    // ADD JUST CHECKBOX
	/*
    QTreeWidgetItem* checkbox = mapLayerGroup->createTreeItem("Do nothing");
    treeItems.append(checkbox);
	*/
  }

  // Settings
  core->addSettingsGroup("testplugin", "Test Plugin Settings");
  testSetting = core->addSetting("testplugin:testsetting", "Value of PI", 3.14);
  testSetting2 = core->addSetting("testplugin:testsetting2",
    "Number of silver squirters", 333);
  testSetting3 = core->addSetting("testplugin:testsetting3",
    "Name of the application", "Updraft");
  hiddenSetting = core->addSetting("testplugin:testsetting", "Not visible",
    "Blablablabla", true);

  testSetting->callOnValueChanged(this, SLOT(showPi()));

  qDebug("testplugin loaded");
}

void TestPlugin::createTab(QString title) {
  QWidget* container = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(container);

  QPushButton* btn1 = new QPushButton("Show help", container);
  QPushButton* btn2 = new QPushButton("Close this tab", container);
  QPushButton* btn3 = new QPushButton("Value of PI", container);

  layout->addWidget(btn1);
  layout->addWidget(btn2);
  layout->addWidget(btn3);

  TabInterface* tab = core->createTab(container, title);

  connect(btn1, SIGNAL(clicked()), this, SLOT(showHelp()));
  // connect(btn2, SIGNAL(clicked()), tab, SLOT(showHelp()));
  tab->connectSlotClose(btn2, SIGNAL(clicked()));
  connect(btn3, SIGNAL(clicked()), this, SLOT(showPi()));
}

void TestPlugin::deinitialize() {
  delete testSetting;

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

void TestPlugin::showPi() {
  QMainWindow* win = core->getMainWindow();

  QString message = "The value of PI is %1";
  message = message.arg(testSetting->get().toFloat());

  QMessageBox::information(win, "Value of PI", message);
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

void TestPlugin::mapLayerDisplayed(bool value, MapLayerInterface* sender) {
  sender->setVisible(value);
  // When the first maplayer is not checked, remove the empty checkbox.
  if (value == false) {
    mapLayerGroup->removeMapLayer(sender);
  }
}

Q_EXPORT_PLUGIN2(testplugin, TestPlugin)

}  // End namespace Updraft

