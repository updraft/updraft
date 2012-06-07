#ifndef UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
#define UPDRAFT_SRC_CORE_PLUGINMANAGER_H_

#include <QtGui>
#include <QDir>

namespace Updraft {

// Forward declarations
class PluginBase;

namespace Core {

/// Top level object that handles plugin loading and operations.
class PluginManager {
 public:
  /// Load all the plugins but don't create core interface for them and
  /// initialize them.
  PluginManager();
  ~PluginManager();

  /// Return pointer to plugin object.
  /// \param name The name of the plugin
  /// \return pointer to the desired plugin.
  PluginBase* getPlugin(const QString &name);

  /// Return directory with plugin data.
  /// \param name The name of the plugin.
  /// \return The directory from which the plugin was loaded.
  QDir getPluginDir(const QString &name);

  /// Return list of all loaded plugins.
  /// \return QVector with all the currently loaded plugins.
  QVector<PluginBase*> getAllPlugins();

 private:
  /// Data belonging to a single loaded plugin.
  struct LoadedPlugin {
    /// Pointer to the plugin instance.
    PluginBase* plugin;
    /// The directory from which the pluign was loaded.
    QDir dir;
  };

  /// Load the plugin, initialize it and put it to the list of
  /// loaded plugins.
  /// \param fileName The file name of the plugin that we want to load
  /// \return Pointer to the PluginBase object that represents the loaded
  ///                 instance of the plugin.
  PluginBase* load(const QString &fileName);

  /// Create core interface, initialize plugin and
  /// place it in the list of loaded plugins.
  /// Logs a debug message about the reason of failure.
  /// \return Pointer to loaded plugin or NULL.
  /// \param obj Loaded plugin before casting to PluginBase.
  ///   Can be NULL if loading failed (this will be logged).
  /// \param dir see the documentation of metho getPluginDir()
  /// \see getPluginDir()
  PluginBase* finishLoading(QObject* obj, const QDir &dir);

  /// Associative map of plugin names to plugin pointers for fast lookup.
  QMap<QString, LoadedPlugin*> plugins;
};

}  // namespace Core
}  // namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PLUGINMANAGER_H_
