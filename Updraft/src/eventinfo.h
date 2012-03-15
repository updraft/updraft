#ifndef UPDRAFT_SRC_EVENTINFO_H_
#define UPDRAFT_SRC_EVENTINFO_H_

namespace Updraft {

enum EventType {
  INVALID = 0,
  LEFT_CLICK,
  RIGHT_CLICK
};

/// This class holds information about a MapObject event.
///
/// This event happens when e.g. the user clicks a MapObject. Subsequently,
/// all the plugins are informed about the event using this class. Everything
/// that the plugins might need to know about the event should go here.
class EventInfo {
 public:
  EventInfo(EventType type_): type(type_) {}

  EventType type;
};

}  // End namespace Updraft

#endif

