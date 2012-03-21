#ifndef UPDRAFT_SRC_CORE_PICKACTION_H_
#define UPDRAFT_SRC_CORE_PICKACTION_H_

#include <QtGui>

namespace Updraft {

class MapObject;
class EventInfo;

namespace Core {

class PickAction: public QAction {
  Q_OBJECT

 public:
  PickAction(MapObject* mapObject, EventInfo* eventInfo);
  ~PickAction() {}

 private slots:
  void sendEventToPlugins();

 private:
  MapObject* mapObject;
  EventInfo* eventInfo;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_PICKACTION_H_

