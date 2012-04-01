#include "taskpoint.h"
#include "plugins/turnpoints/turnpoint.h"

namespace Updraft {

QString TaskPoint::getCode() const {
  return code;
}

QString TaskPoint::getName() const {
  return name;
}

const Util::Location& TaskPoint::getLocation() const {
  return location;
}

void TaskPoint::setTP(const TurnPoint *tp) {
  if (tp == NULL) {
    return;
  }

  code = tp->code;
  name = tp->name;
  location = tp->location;
}

void TaskPoint::setLocation(const Util::Location &location_) {
  location = location_;
}

void TaskPoint::setAssignedArea(const AssignedArea &assignedArea_) {
  assignedArea = assignedArea_;
}

}  // End namespace Updraft
