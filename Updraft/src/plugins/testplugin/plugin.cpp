#include "plugin.h"

#include <QPushButton>
#include <QHBoxLayout>

#include "../../coreinterface.h"
#include "../../core/ui/maplayergroup.h"

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
    mapLayerGroup->insertMapLayer(0, (osg::Node*)0, "Layer A");
    mapLayerGroup->insertMapLayer(1, (osg::Node*)1, "Layer B");

    // Connect display/hide signals with slots.
    connect(mapLayerGroup, SIGNAL(mapLayerDisplayed(osg::Node*)),
            this, SLOT(mapLayerDisplayed(osg::Node*)));
    connect(mapLayerGroup, SIGNAL(mapLayerHidden(osg::Node*)),
            this, SLOT(mapLayerHidden(osg::Node*)));
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

void TestPlugin::mapLayerDisplayed(osg::Node* layer) {
  qDebug("testplugin: Map layer displayed.");
}

void TestPlugin::mapLayerHidden(osg::Node* layer) {
  qDebug("testplugin: Map layer hidden.");
}

Q_EXPORT_PLUGIN2(testplugin, TestPlugin)

}  // End namespace Updraft

