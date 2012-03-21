#include "pickaction.h"
#include "updraft.h"
#include "pluginmanager.h"
#include "../pluginbase.h"
#include "../mapobject.h"
#include "../eventinfo.h"

namespace Updraft {
namespace Core {

PickAction::PickAction(
  MapObject* mapObject,
  EventInfo* eventInfo):
  QAction(mapObject->name, NULL),
  mapObject(mapObject),
  eventInfo(eventInfo) {
  connect(this, SIGNAL(triggered), this, SLOT(sendEventToPlugins));
}

void PickAction::sendEventToPlugins() {
  foreach(PluginBase* plugin, updraft->pluginManager->getAllPlugins()) {
    plugin->handleMouseEvent(mapObject, eventInfo);
  }
}

}  // End namespace Core
}  // End namespace Updraft

