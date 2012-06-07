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
  PickAction(MapObject* mapObject, const EventInfo& eventInfo);
  ~PickAction() {}

  int isEmpty() { return pluginList.size() == 0; }
  void pushPlugin(PluginBase* plugin);

 private slots:
  void sendEventToPlugins();

 private:
  MapObject* mapObject;
  EventInfo eventInfo;
  QList<PluginBase*> pluginList;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PICKACTION_H_

