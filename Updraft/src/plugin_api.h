#ifndef UPDRAFT_PLUGIN_API_H_
#define UPDRAFT_PLUGIN_API_H_

#include <QString>
#include <QtPlugin>

#define PLUGIN_API_VERSION 0

namespace Updraft {

namespace Core { class PluginManager; }

class CoreInterface {
  /// \todo Fill this once plugin API is finished
 private:
  CoreInterface(IPlugin *pl) :
    plugin(pl) {}

  IPlugin plugin;
};

class IPlugin {
 public:
  /// Plugin metadata.
  /// \{
  virtual QString getName() = 0;

  /// Priority gives the order in which plugins receive callbacks.
  virtual unsigned getPriority() = 0;

  /// Plugin's API version.
  virtual unsigned getPluginApiVersion() { return PLUGIN_API_VERSION }
  /// \}

  /// Called after the plugin is loaded.
  virtual void initialize(Core::CoreInterface *) = 0;

  /// Called before the plugin is unloaded.
  virtual void deinitialize() = 0;

  /// \todo Fill this once plugin API is finished
};

} // namespace Updraft

Q_DECLARE_INTERFACE(Updraft::IPlugin, "Updraft::IPlugin")

#endif // UPDRAFT_IPLUGIN_H_
