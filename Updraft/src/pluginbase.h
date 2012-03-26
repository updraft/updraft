#ifndef UPDRAFT_SRC_PLUGINBASE_H_
#define UPDRAFT_SRC_PLUGINBASE_H_

#include <QtGui>
#include <QStringList>

#include "coreinterface.h"
#include "tabinterface.h"
#include "menuinterface.h"

#define PLUGIN_API_VERSION 0

namespace Updraft {

class EventInfo;

// Forward declarations
namespace Core {
  class UpdraftParent;
  class PluginManager;
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

  /// Called when a MapObject is right-clicked on a map.
  virtual void fillContextMenu(MapObject* obj, MenuInterface* menu) {}

  /// Tells whether this plugin wants to handle a mouse click event.
  virtual bool wantsToHandleClick(MapObject* obj) { return false; }

  /// Handles the left mouse click event.
  virtual void handleClick(MapObject* obj, const EventInfo* evt) {}

  /// Callback to open a file.
  /// \return Was opening successful?
  /// \param filename full path to a file
  /// \param roleId identification of role being opened
  virtual bool fileOpen(const QString &filename, int roleId) {
    return false; }

  void setCoreInterface(CoreInterface *coreInterface) {
    core = coreInterface;
  }

  CoreInterface* getCoreInterface() {
    return core;
  }

 private:
  /// Disallow copying
  PluginBase(const PluginBase& other) {}

 protected:
  /// This serves for calling CoreInterface methods
  CoreInterface *core;
};

}  // namespace Updraft

Q_DECLARE_INTERFACE(Updraft::PluginBase, "Updraft_PluginBase")

#endif  // UPDRAFT_SRC_PLUGINBASE_H_
