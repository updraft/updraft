#ifndef UPDRAFT_SRC_CORE_PICKACTION_H_
#define UPDRAFT_SRC_CORE_PICKACTION_H_

#include <QtGui>
#include <QList>

#include "../eventinfo.h"

namespace Updraft {

class MapObject;
class PluginBase;

namespace Core {

/// Action that gets called when a MapObject is clicked.
/// Also appears in mouse picking disambiguation menu.
class PickAction: public QAction {
  Q_OBJECT

 public:
  /// Creates the action and sets the clicked map object and the click
  /// event info.
  /// \param mapObject The map object that was clicked.
  /// \param eventInfo Information obout the click event
  PickAction(MapObject* mapObject, const EventInfo& eventInfo);
  ~PickAction() {}

  /// Tells whether there are any plugins who wanted to handle this action.
  /// \return True if any of the plugins wanted to handle the action.
  int isEmpty() { return pluginList.size() == 0; }

  /// Adds the given plugin to the list of plugins who want to handle this
  /// action.
  /// \param plugin The plugin who wants to handle this action.
  void pushPlugin(PluginBase* plugin);

 private slots:
  /// Sends the stored click event to the interested plugins.
  void sendEventToPlugins();

 private:
  /// The map object that was clicked.
  MapObject* mapObject;

  /// Event info about the click event.
  EventInfo eventInfo;

  /// List of the plugins that want to handle the event.
  QList<PluginBase*> pluginList;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PICKACTION_H_

