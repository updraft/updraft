#ifndef UPDRAFT_PLUGIN_API_H_
#define UPDRAFT_PLUGIN_API_H_

#include <QtGui>
#include "coreinterface.h"

#define PLUGIN_API_VERSION 0

namespace Updraft {

// Forward declarations
namespace Core {
  class Tab;
  class FileType;
}

/// Base class for plugins.
/// All mehtods of this class must be pure virtual or inline!
class PluginBase {
 public:
  PluginBase() {}
  virtual ~PluginBase() {}

  /// Plugin metadata.
  /// \{
  virtual QString getName() = 0;

  /// Priority gives the order in which plugins receive callbacks.
  virtual unsigned getPriority() = 0;

  /// Plugin's API version.
  virtual unsigned getPluginApiVersion() { return PLUGIN_API_VERSION; }
  /// \}

  /// Called after the plugin is loaded.
  virtual void initialize() = 0;

  /// Called before the plugin is unloaded.
  virtual void deinitialize() = 0;

  virtual void tabSelected(Core::Tab *tab) {}
  virtual void tabDeselected(Core::Tab *tab) {}

  /// Called before the tab is closed.
  virtual void tabClosed(Core::Tab *tab) {}

  /// Callback to open a file.
  /// \return Was opening successful?
  /// \param roles List of role indices (roles are
  ///   returned by fileIdentification().)
  virtual bool fileOpen(QString filename, QList<int> roles) { return false; }

  /// Asking a plugin to identify data in a file.
  /// \return List of role descriptions.
  /// \see fileOpen()
  virtual QList<QString> fileIdentification(QString filename) {
    return QList<QString>();
  }

  /// \todo Fill this once plugin API is finished

  void setCoreInterface(CoreInterface *coreInterface) {
    core = coreInterface;
  }

 private:
  //Disallow copying
  PluginBase(const PluginBase& other) {};

 protected:
  //This serves for calling CoreInterface methods
  CoreInterface *core;

};

}  // namespace Updraft

Q_DECLARE_INTERFACE(Updraft::PluginBase, "Updraft_PluginBase")

#endif  // UPDRAFT_IPLUGIN_H_
