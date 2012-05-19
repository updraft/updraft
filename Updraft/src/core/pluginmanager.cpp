#include "pluginmanager.h"

#include <QDebug>
#include <QLibrary>
#include <QtGui>
#include <QFileInfo>

#include "../pluginbase.h"
#include "coreimplementation.h"

namespace Updraft {
namespace Core {

PluginManager::PluginManager() {
  qDebug("Searching for plugins in plugin directory.");

  QDir plugins(QCoreApplication::applicationDirPath());
  if (!plugins.cd("plugins")) {
    qDebug("Could not find 'plugins' directory");
    return;
  }

  QStringList pluginDirs = plugins.entryList(
    QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);

  QStringList pluginFiles;

  foreach(QString pluginDir, pluginDirs) {
    plugins.cd(pluginDir);
    QStringList files = plugins.entryList(QDir::Files | QDir::Readable);

    foreach(QString fileName, files) {
      QString path = plugins.absolutePath() + "/" + fileName;
      if (QLibrary::isLibrary(path)) {
        qDebug() << "Found plugin" << path;
        pluginFiles.append(path);
      }
    }
    plugins.cdUp();
  }

  QPluginLoader loader;
  foreach(QString pluginPath, pluginFiles) {
    loader.setFileName(pluginPath);
    loader.load();
  }

  foreach(QString pluginPath, pluginFiles) {
    load(pluginPath);
  }
}

PluginManager::~PluginManager() {
  foreach(LoadedPlugin* p, plugins.values()) {
    p->plugin->deinitialize();
    delete p->plugin;
    delete p;
  }
}

PluginBase* PluginManager::load(const QString &fileName) {
  qDebug() << "Loading plugin" << fileName;
  QPluginLoader loader(fileName);

  QObject *pluginInstance = loader.instance();
  if (!pluginInstance) {
    qDebug() << "Loading plugin failed. (" << loader.errorString() << ")";
    return NULL;
  }

  PluginBase* plugin = qobject_cast<PluginBase*>(pluginInstance);
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

  LoadedPlugin* lp = new LoadedPlugin;
  lp->plugin = plugin;
  lp->dir = QFileInfo(fileName).absoluteDir();

  plugins.insert(plugin->getName(), lp);

  return plugin;
}

void PluginManager::finishInit() {
  foreach(LoadedPlugin *lp, plugins.values()) {
    lp->plugin->initialize(new CoreImplementation(lp->plugin));
  }
}

PluginBase* PluginManager::getPlugin(const QString &name) {
  LoadedPlugin* lp = plugins.value(name);
  if (!lp) return NULL;

  return lp->plugin;
}

QDir PluginManager::getPluginDir(const QString &name) {
  LoadedPlugin* lp = plugins.value(name);
  if (!lp) return QDir();

  return lp->dir;
}

QVector<PluginBase*> PluginManager::getAllPlugins() {
  QVector<PluginBase*> ret;

  foreach(LoadedPlugin *p, plugins.values()) {
    ret.append(p->plugin);
  }

  return ret;
}

}  // namespace Core
}  // namespace Updraft
