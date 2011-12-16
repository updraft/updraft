#ifndef UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
#define UPDRAFT_SRC_CORE_PLUGINMANAGER_H_

#include <QtGui>

namespace Updraft {

// Forward declarations
class PluginBase;

namespace Core {

class UpdraftParent;

class PluginManager {
 public:
  explicit PluginManager(UpdraftParent* setParent);
  ~PluginManager();

  PluginBase* load(QString fileName);
  void unload(QString name);

  PluginBase* getPlugin(QString name);
  QVector<PluginBase*> getAllPlugins();

 private:
  struct LoadedPlugin {
    QPluginLoader* loader;
    PluginBase* plugin;
    // CoreInterface* coreinterface; XXX: I don't thik this is needed. Cestmir
  };

  PluginBase* finishLoading(QPluginLoader* loader, QObject* obj);

  LoadedPlugin* findByPointer(PluginBase *pointer);

  QHash<QString, LoadedPlugin*> plugins;

  UpdraftParent* parent;
};

}  // namespace Core
}  // namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
