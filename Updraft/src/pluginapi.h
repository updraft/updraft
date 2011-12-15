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

class IPlugin {
 public:
  IPlugin() {};
  virtual ~IPlugin() {};

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
  virtual bool fileOpen(QString filename) { return false; }

  /// \todo Fill this once plugin API is finished

  void initializeCoreInterface(Core::UpdraftParent* parent) {
    core = CoreInterface(parent, this);
  }

 private:
  //Disallow copying
  IPlugin(const IPlugin& other) {};

 protected:
  //This serves for calling CoreInterface methods
  CoreInterface core;

};

}  // namespace Updraft

Q_DECLARE_INTERFACE(Updraft::IPlugin, "Updraft_IPlugin")

#endif  // UPDRAFT_IPLUGIN_H_
