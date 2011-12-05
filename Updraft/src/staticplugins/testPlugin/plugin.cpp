#include "plugin.h"

#include <QObject>

namespace Updraft {

TestPlugin::TestPlugin() {}

QString TestPlugin::getName() {
  return "TestPlugin";
}

unsigned TestPlugin::getPriority() {
  return 1;
}

void TestPlugin::initialize() {
  qDebug("TestPlugin loaded");
}

void TestPlugin::deinitialize() {
  qDebug("TestPlugin unloaded");
}

Q_EXPORT_PLUGIN2(TestPlugin, TestPlugin)

}  //End namespace Updraft

