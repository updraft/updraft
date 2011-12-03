#ifndef UPDRAFT_CORE_PLUGINMANAGER_H_
#define UPDRAFT_CORE_PLUGINMANAGER_H_

#include <QLinkedList>

namespace Updraft {

class IPlugin {};

namespace Core {

class PluginManager{
 public:
  PluginManager();
  ~PluginManager();

  IPlugin* load(QString fileName);
  void unload(QString name);

  IPlugin* getPlugin(QString name);
  QVector<IPlugin*> getAllPlugins();

 private:
  struct LoadedPlugin {
    QPluginLoader* loader;
    IPlugin* plugin;
    CoreInterface* coreinterface;
  };

  IPlugin* finishLoading(LoadedPlugin* p, QObject* obj);

  LoadedPlugin* findByName(QString name);
  LoadedPlugin* findByPointer(IPlugin *pointer);

  QVector<LoadedPlugin*> plugins;
};

} // namespace Core
} // namespace Updraft

#endif // UPDRAFT_CORE_PLUGINMANAGER_H_
