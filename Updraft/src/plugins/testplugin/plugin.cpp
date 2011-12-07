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

  core.getSystemMenu(MENU_HELP)->insertAction(0, helpAction);

  qDebug("testplugin loaded");
}

void TestPlugin::deinitialize() {
  delete helpAction;

  qDebug("testplugin unloaded");
}

void TestPlugin::showHelp() {
  QMainWindow* win = core.getMainWindow();

  QMessageBox::information(win, "About testplugin",
    "Testplugin is just a test plugin to see whether our api is working");
}

Q_EXPORT_PLUGIN2(testplugin, TestPlugin)

}  // End namespace Updraft

