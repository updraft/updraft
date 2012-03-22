#ifndef UPDRAFT_SRC_CORE_PICKACTION_H_
#define UPDRAFT_SRC_CORE_PICKACTION_H_

#include <QtGui>
#include <QList>

namespace Updraft {

class MapObject;
class EventInfo;
class PluginBase;

namespace Core {

class PickAction: public QAction {
  Q_OBJECT

 public:
  PickAction(MapObject* mapObject, EventInfo* eventInfo);
  ~PickAction() {}

  int isEmpty() { return pluginList.size() == 0; }
  void pushPlugin(PluginBase* plugin);

 private slots:
  void sendEventToPlugins();

 private:
  MapObject* mapObject;
  EventInfo* eventInfo;
  QList<PluginBase*> pluginList;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PICKACTION_H_

