#include "plugin.h"

#include <QPushButton>
#include <QHBoxLayout>

#include "../../core/ui/tab.h"

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

  createTab("Button tab 1");
  createTab("Button tab 2");

  qDebug("testplugin loaded");
}

void TestPlugin::createTab(QString title) {
  QWidget* container = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(container);

  QPushButton* btn1 = new QPushButton("Show help", container);
  QPushButton* btn2 = new QPushButton("Close this tab", container);

  layout->addWidget(btn1);
  layout->addWidget(btn2);

  Core::Tab* tab = core.createTab(container, title);

  connect(btn1, SIGNAL(clicked()), this, SLOT(showHelp()));
  // connect(btn2, SIGNAL(clicked()), tab, SLOT(showHelp()));
  connect(btn2, SIGNAL(clicked()), tab, SLOT(close()));
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

