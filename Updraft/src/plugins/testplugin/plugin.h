#ifndef UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_
#define UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_

#include <QtGui>

#include "../../pluginapi.h"

namespace Updraft {

class Q_DECL_EXPORT TestPlugin: public QObject, public IPlugin {
  Q_OBJECT
  Q_INTERFACES(Updraft::IPlugin)

 public:
  TestPlugin();

  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();

 public slots:
  void showHelp();

 private:
  void createTab(QString title);

  QAction* helpAction;
};
}

#endif  // UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_

