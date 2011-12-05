#ifndef UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
#define UPDRAFT_SRC_CORE_PLUGINMANAGER_H_

#include <QtGui>

namespace Updraft {

// Forward declarations
class IPlugin;

namespace Core {

class UpdraftParent;

class PluginManager {
 public:
  explicit PluginManager(UpdraftParent* setParent);
  ~PluginManager();

  IPlugin* load(QString fileName);
  void unload(QString name);

  IPlugin* getPlugin(QString name);
  QVector<IPlugin*> getAllPlugins();

 private:
  struct LoadedPlugin {
    QPluginLoader* loader;
    IPlugin* plugin;
    // CoreInterface* coreinterface; XXX: I don't thik this is needed. Cestmir
  };

  IPlugin* finishLoading(QPluginLoader* loader, QObject* obj);

  LoadedPlugin* findByName(QString name);
  LoadedPlugin* findByPointer(IPlugin *pointer);

  QVector<LoadedPlugin*> plugins;

  UpdraftParent* parent;
};

}  // namespace Core
}  // namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
