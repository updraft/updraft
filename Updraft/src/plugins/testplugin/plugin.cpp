#include "plugin.h"
#include "../../coreinterface.h"

namespace Updraft {

TestPlugin::TestPlugin() {}

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

  core.getSystemMenu(MENU_HELP)->insertAction(1, helpAction);
  core.getSystemMenu(MENU_HELP)->insertAction(0, helpAction2);

  qDebug("testplugin loaded");

  // Create a new menu and insert a test action into it as well
  myMenu = core.createMenu("Test plugin");

  helpAction3 = new QAction("About testplugin...", this);
  connect(helpAction3, SIGNAL(triggered()), this, SLOT(showHelp()));

  myMenu->insertAction(0, helpAction3);
}

void TestPlugin::deinitialize() {
  delete helpAction;

  core.removeMenu(myMenu);

  qDebug("testplugin unloaded");
}

void TestPlugin::showHelp() {
  QMainWindow* win = core.getMainWindow();

  QMessageBox::information(win, "About testplugin",
    "Testplugin is just a test plugin to see whether our api is working");
}

Q_EXPORT_PLUGIN2(testplugin, TestPlugin)

}  // End namespace Updraft

