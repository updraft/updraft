#ifndef UPDRAFT_SRC_STATICPLUGINS_TESTPLUGIN_PLUGIN_H_
#define UPDRAFT_SRC_STATICPLUGINS_TESTPLUGIN_PLUGIN_H_

#include "../../pluginapi.h"

#include <QObject>

namespace Updraft {

class TestPlugin: public QObject, public IPlugin {
  Q_OBJECT
  Q_INTERFACES(Updraft::IPlugin)

 public:
  TestPlugin();

  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();
};

}

#endif

