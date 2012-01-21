#ifndef UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_
#define UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_

#include <QtGui>
#include <QMap>

#include "../../pluginbase.h"
#include "../../maplayerinterface.h"

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

  bool fileOpen(const QString &filename, int roleId);
  void fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename);

 public slots:
  void showHelp();
  void mapLayerDisplayed(int id, bool value);

 private:
  void createTab(QString title);

  QAction* helpAction;
  QAction* helpAction2;
  QAction* helpAction3;

  MenuInterface* myMenu;

  MapLayerGroup *mapLayerGroup;
  QMap<int, Updraft::MapLayerInterface*> mapLayers;
};
}

#endif  // UPDRAFT_SRC_PLUGINS_TESTPLUGIN_PLUGIN_H_

