#ifndef UPDRAFT_SRC_PLUGINBASE_H_
#define UPDRAFT_SRC_PLUGINBASE_H_

#include <QtGui>
#include <QStringList>

#include "coreinterface.h"
#include "tabinterface.h"
#include "menuinterface.h"

#define PLUGIN_API_VERSION 0

namespace Updraft {

#ifndef UPDRAFT_CORE
// Pointer to coreinterface, defined in each plugin.
// To access this pointer from any file of plugin, include pluginbase.h.
extern CoreInterface *g_core;
#endif

class EventInfo;

/// Base class for plugins.
/// All mehtods of this class must be pure virtual or inline!
class PluginBase {
 public:
  PluginBase() {}
  virtual ~PluginBase() {}

  /// Plugin metadata.
  /// \{
  virtual QString getName() = 0;

  /// Plugin's API version.
  virtual unsigned getPluginApiVersion() { return PLUGIN_API_VERSION; }
  /// \}

  /// Called after the plugin is loaded.
  /// \param coreInterface pointer to assigned CoreInterface
  /// Plugin should store this value in g_core.
  virtual void initialize(CoreInterface *coreInterface) = 0;

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

  /// Callback asking the plugin if closing all files is Ok.
  /// The plug-in may display a dialog box and ask the user.
  /// \return true if this plugin has no objections to closing the application.
  virtual bool askClose() { return true; }

 private:
  /// Disallow copying
  PluginBase(const PluginBase& other) {}
};

}  // namespace Updraft

Q_DECLARE_INTERFACE(Updraft::PluginBase, "Updraft_PluginBase")

#endif  // UPDRAFT_SRC_PLUGINBASE_H_
