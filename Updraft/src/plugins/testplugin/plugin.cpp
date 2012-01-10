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

  core->registerFiletype(".txt",
    "Just a text file; in all categories", CATEGORY_ANY);
  core->registerFiletype(".png",
    "Png image. Maybe a map?", CATEGORY_PERSISTENT);
  core->registerFiletype(".html",
    "bzzzz", CATEGORY_TEMPORARY);

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

  // Settings
  core->addSettingsGroup("testplugin", "Test Plugin");
  testSetting = core->addSetting("testplugin:testsetting", "Value of PI", 3.14);
  testSetting2 = core->addSetting("testplugin:testsetting2",
    "Number of silver squirters", 333);
  testSetting3 = core->addSetting("testplugin:testsetting3",
    "Name of the application", "Updraft");

  connect(testSetting, SIGNAL(valueChanged()), this, SLOT(showPi()));

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

void TestPlugin::showPi() {
  QMainWindow* win = core->getMainWindow();

  QString message = "The value of PI is %1";
  message = message.arg(testSetting->get().toFloat());

  QMessageBox::information(win, "Value of PI", message);
}

bool TestPlugin::fileOpen(QString filename, QList<int> roles) {
  foreach(int role, roles) {
    qDebug() << "Testplugin opens file " << filename << ", role " << role;
  }

  return true;
}

QStringList TestPlugin::fileIdentification(QString filename) {
  QStringList ret;
  if (filename.endsWith("txt")) {
    ret.append("Txt file, role 1");
    ret.append("Txt file, role 2");
    ret.append("Txt file, role 3");
  } else {
    ret.append("other file, the only role");
  }
  return ret;
}

void TestPlugin::mapLayerDisplayed(osg::Node* layer) {
  qDebug("testplugin: Map layer displayed.");
}

void TestPlugin::mapLayerHidden(osg::Node* layer) {
  qDebug("testplugin: Map layer hidden.");
}

Q_EXPORT_PLUGIN2(testplugin, TestPlugin)

}  // End namespace Updraft

