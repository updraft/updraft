#ifndef UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
#define UPDRAFT_SRC_CORE_PLUGINMANAGER_H_

#include <QtGui>
#include <QDir>

namespace Updraft {

// Forward declarations
class PluginBase;

namespace Core {

class UpdraftParent;

class PluginManager {
 public:
  /// Load all the plugins but don't create core interface for them and
  /// don't initialize them yet.
  /// \note finishInit(); must be called after both PluginManager and
  /// TranslationManager are created!
  PluginManager();
  ~PluginManager();

  /// Finalize the initialization.
  /// Used to break the circular dependency between plugin manager
  /// and translation manager.
  /// This method must be called after all other top level objects
  /// are ready to avoid problems with not yet working core interface methods.
  void finishInit();

  /// Return pointer to plugin object.
  PluginBase* getPlugin(const QString &name);

  /// Return directory with plugin data.
  QDir getPluginDir(const QString &name);

  /// Return list of all loaded plugins.
  QVector<PluginBase*> getAllPlugins();

 private:
  struct LoadedPlugin {
    PluginBase* plugin;
    QDir dir;
  };

  /// Load the plugin, initialize it and put it to the list of
  /// loaded plugins.
  PluginBase* load(const QString &fileName);

  /// Create core interface, initialize plugin and
  /// place it in the list of loaded plugins.
  /// Logs a debug message about the reason of failure.
  /// \return Pointer to loaded plugin or NULL.
  /// \param obj Loaded plugin before casting to PluginBase.
  ///   Can be NULL if loading failed (this will be logged).
  /// \param dir \see getPluginDir()
  PluginBase* finishLoading(QObject* obj, const QDir &dir);

  QMap<QString, LoadedPlugin*> plugins;
};

}  // namespace Core
}  // namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
