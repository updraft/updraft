#include "core/pluginmanager.h"

#include <QDebug>
#include <QPluginLoader>

#include "core/coreinterface.h"
#include "plugin_api.h"

namespace Updraft {
namespace Core {

/// Constructor loads static plugins.
PluginManager::PluginManager() {
  qDebug("Loading static plugins.");

  foreach(QObject* obj, QPluginLoader::staticInstances()) {
    finishLoading(NULL, obj);
  }
}

PluginManager::~PluginManager() {

}

/// Load the plugin, initialize it and put it to the list of
/// loaded plugins.
IPlugin* PluginManager::load(QString fileName) {
  QPluginLoader* loader = new QPluginLoader(fileName);
  return finishLoading(loader, loader->instance());
}

void PluginManager::unload(QString name){

}

IPlugin* PluginManager::getPlugin(QString name) {
  foreach(LoadedPlugin *p, plugins) {
    if(p->getName() == name) {
      return p->plugin;
    }
  }

  return NULL;
}

QVector<IPlugin*> PluginManager::getAllPlugins() {
  QVector ret;

  foreach(LoadedPlugin *p, plugins) {
    ret.append(p->plugin);
  }

  return ret;
}

/// Create core interface, initialize plugin and 
/// place it in the list of loaded plugins.
/// Logs a debug message about the reason of failure.
/// \return Pointer to loaded plugin or NULL.
/// \param loader Plugin loader that was used for getting the plugin
///   or NULL for static plugins.
/// \param obj Loaded plugin before casting to IPlugin.
///   Can be NULL if loading failed (this will be logged).
IPlugin* PluginManager::finishLoading(QPluginLoader* loader, QObject* obj) {
  if(!obj) {
    qDebug("Loading plugin failed.");
    return NULL;
  }

  LoadedPlugin* lp = new LoadedPlugin;
  lp->loader = loader;

  lp->plugin = qobject_cast<IPlugin*>(obj);

  if(!lp->plugin) {
    qDebug("Plugin doesn't have correct interface.");
    delete lp;
    return NULL;
  }

  unsigned apiVersion = p->plugin->getPluginApiVersion();

  if(apiVersion != PLUGIN_API_VERSION) {
    qDebug("Wrong version of plugin API (expected %d, got %d).",
      PLUGIN_API_VERSION, apiVersion);
    delete lp;
    return NULL;
  }

  p->coreinterface = new CoreInterface(lp->plugin);

  plugin->initialize(ci);

  plugins.append(lp);

  return lp->plugin;
}

LoadedPlugin* PluginManager::findByName(QString name) {
  foreach(LoadedPlugin *p, plugins) {
    if(p->plugin->getName() == name) {
      return p;
    }
  }

  return NULL;
}

LoadedPlugin* PluginManager::findByPointer(IPlugin* pointer) {
  foreach(LoadedPlugin *p, plugins) {
    if(p->plugin == pointer) {
      return p;
    }
  }

  return NULL;
}

} // namespace Core
} // namespace Updraft
