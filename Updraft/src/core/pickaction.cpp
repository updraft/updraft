#include "pickaction.h"
#include "updraft.h"
#include "pluginmanager.h"
#include "../pluginbase.h"
#include "../mapobject.h"

namespace Updraft {
namespace Core {

PickAction::PickAction(
  MapObject* mapObject,
  const EventInfo& eventInfo):
  QAction(mapObject->name, NULL),
  mapObject(mapObject),
  eventInfo(eventInfo) {
  connect(this, SIGNAL(triggered()), this, SLOT(sendEventToPlugins()));
}

void PickAction::pushPlugin(PluginBase* plugin) {
  pluginList.push_back(plugin);
}

void PickAction::sendEventToPlugins() {
  foreach(PluginBase* plugin, pluginList) {
    plugin->handleClick(mapObject, &eventInfo);
  }
}

}  // End namespace Core
}  // End namespace Updraft

