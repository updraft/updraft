#ifndef UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_
#define UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_

#include <QtGui>

#include "../../pluginbase.h"

namespace osg {
  class Node;
}

namespace Updraft {

class Q_DECL_EXPORT TestPlugin: public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  TestPlugin();
  virtual ~TestPlugin();

  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();

  bool fileOpen(QString filename, QList<int> roles);
  QStringList fileIdentification(QString filename);

 public slots:
  void showHelp();
  void mapLayerDisplayed(osg::Node* layer);
  void mapLayerHidden(osg::Node* layer);

 private:
  void createTab(QString title);

  QAction* helpAction;
  QAction* helpAction2;
  QAction* helpAction3;

  MenuInterface* myMenu;

  MapLayerGroup *mapLayerGroup;
};
}

#endif  // UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_

