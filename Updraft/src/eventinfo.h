#ifndef UPDRAFT_SRC_EVENTINFO_H_
#define UPDRAFT_SRC_EVENTINFO_H_

namespace Updraft {

enum EventType {
  INVALID = 0,
  LEFT_CLICK,
  RIGHT_CLICK
};

class EventInfo {
 public:
  EventInfo(EventType type_): type(type_) {}

  EventType type;
};

}  // End namespace Updraft

#endif

