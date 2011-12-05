#include "core/pluginmanager.h"

#include <QtGui>

#include "pluginapi.h"

namespace Updraft {
namespace Core {

/// Constructor loads static plugins.
PluginManager::PluginManager(UpdraftParent* setParent): parent(setParent) {
  qDebug(QT_TR_NOOP("Loading static plugins."));

  foreach(QObject* obj, QPluginLoader::staticInstances()) {
    finishLoading(NULL, obj);
  }
}

PluginManager::~PluginManager() {}

/// Load the plugin, initialize it and put it to the list of
/// loaded plugins.
IPlugin* PluginManager::load(QString fileName) {
  QPluginLoader* loader = new QPluginLoader(fileName);
  return finishLoading(loader, loader->instance());
}

void PluginManager::unload(QString name) {}

IPlugin* PluginManager::getPlugin(QString name) {
  foreach(LoadedPlugin *p, plugins) {
    if (p->plugin->getName() == name) {
      return p->plugin;
    }
  }

  return NULL;
}

QVector<IPlugin*> PluginManager::getAllPlugins() {
  QVector<IPlugin*> ret;

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
  if (!obj) {
    qDebug(QT_TR_NOOP("Loading plugin failed."));
    return NULL;
  }

  LoadedPlugin* lp = new LoadedPlugin;
  IPlugin* plugin = qobject_cast<IPlugin*>(obj);

  if (!plugin) {
    qDebug(QT_TR_NOOP("Plugin doesn't have correct interface."));
    return NULL;
  }

  unsigned apiVersion = plugin->getPluginApiVersion();

  if (apiVersion != PLUGIN_API_VERSION) {
    qDebug(QT_TR_NOOP("Wrong version of plugin API (expected %d, got %d)."),
      PLUGIN_API_VERSION, apiVersion);
    return NULL;
  }

  lp->loader = loader;
  lp->plugin = plugin;

  plugin->initializeCoreInterface(parent);
  plugin->initialize();

  plugins.append(lp);

  return plugin;
}

PluginManager::LoadedPlugin* PluginManager::findByName(QString name) {
  foreach(LoadedPlugin *p, plugins) {
    if (p->plugin->getName() == name) {
      return p;
    }
  }

  return NULL;
}

PluginManager::LoadedPlugin* PluginManager::findByPointer(IPlugin* pointer) {
  foreach(LoadedPlugin *p, plugins) {
    if (p->plugin == pointer) {
      return p;
    }
  }

  return NULL;
}

}  // namespace Core
}  // namespace Updraft
