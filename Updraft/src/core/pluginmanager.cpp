#include "pluginmanager.h"

#include <QtGui>

#include "../pluginapi.h"

namespace Updraft {
namespace Core {

/// Constructor loads static plugins.
PluginManager::PluginManager(UpdraftParent* setParent): parent(setParent) {
  qDebug("Searching for plugins in plugin directory.");

  QDir plugins(QCoreApplication::applicationDirPath());
  if (!plugins.cd("plugins")) {
    qDebug("Could not find 'plugins' directory");
    return;
  }

  QStringList pluginDirs = plugins.entryList
    (QDir::Dirs | QDir::NoDotAndDotDot);

  QStringList dllsMask;
  dllsMask.push_back(QString("*.so"));
  dllsMask.push_back(QString("*.dll"));

  foreach(QString pluginDir, pluginDirs) {
    plugins.cd(pluginDir);
    QStringList dlls = plugins.entryList(dllsMask);

    // Not sure how else to search for plugins:
    if (!dlls.empty()) {
      load(plugins.absolutePath() + "/" + dlls.front());
    }
    plugins.cdUp();
  }
}

PluginManager::~PluginManager() {
  foreach(LoadedPlugin* p, plugins.values()) {
    p->plugin->deinitialize();
  }
}

/// Load the plugin, initialize it and put it to the list of
/// loaded plugins.
IPlugin* PluginManager::load(QString fileName) {
  qDebug("Loading plugin %s", fileName.toAscii().data());
  QPluginLoader* loader = new QPluginLoader(fileName);
  QObject *pluginInstance = loader->instance();
  qDebug("Loading error report: %s ", loader->errorString().toAscii().data());
  return finishLoading(loader, pluginInstance);
}

void PluginManager::unload(QString name) {
  LoadedPlugin* lp = plugins.value(name);
  if (!lp) return;

  lp->plugin->deinitialize();
  plugins.remove(name);
}

IPlugin* PluginManager::getPlugin(QString name) {
  LoadedPlugin* lp = plugins.value(name);
  if (!lp) return NULL;

  return lp->plugin;
}

QVector<IPlugin*> PluginManager::getAllPlugins() {
  QVector<IPlugin*> ret;

  foreach(LoadedPlugin *p, plugins.values()) {
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
    qDebug("Loading plugin failed.");
    return NULL;
  }

  LoadedPlugin* lp = new LoadedPlugin;
  IPlugin* plugin = qobject_cast<IPlugin*>(obj);

  if (!plugin) {
    qDebug("Plugin doesn't have correct interface.");
    return NULL;
  }

  unsigned apiVersion = plugin->getPluginApiVersion();

  if (apiVersion != PLUGIN_API_VERSION) {
    qDebug("Wrong version of plugin API (expected %d, got %d).",
      PLUGIN_API_VERSION, apiVersion);
    return NULL;
  }

  if (plugins.contains(plugin->getName())) {
    qDebug("Plugin named %s already loaded",
      plugin->getName().toAscii().data());
    return NULL;
  }

  lp->loader = loader;
  lp->plugin = plugin;

  plugin->initializeCoreInterface(parent);
  plugin->initialize();

  plugins.insert(plugin->getName(), lp);

  return plugin;
}

PluginManager::LoadedPlugin* PluginManager::findByPointer(IPlugin* pointer) {
  foreach(LoadedPlugin *p, plugins.values()) {
    if (p->plugin == pointer) {
      return p;
    }
  }

  return NULL;
}

}  // namespace Core
}  // namespace Updraft
