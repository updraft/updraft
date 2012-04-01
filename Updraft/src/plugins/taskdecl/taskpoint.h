#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKPOINT_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKPOINT_H_

#include "assignedarea.h"
#include "util/location.h"

namespace Updraft {

struct TurnPoint;

class TaskPoint {
 public:
  /// \return Code of TaskPoint or empty string if it doesn't exist
  QString getCode() const;

  /// \return Name of TaskPoint or empty string if it doesn't exist
  QString getName() const;

  /// \return Geographical location of the task point
  const Util::Location& getLocation() const;

  /// Gathers data from a TurnPoint and store them in the TaskPoint.
  void setTP(const TurnPoint *tp);

  /// Sets the new location. Deletes information about assigned TurnPoint.
  void setLocation(const Util::Location &location_);

  /// Sets shape of the assigned area.
  void setAssignedArea(const AssignedArea &assignedArea_);

 private:
  /// Code identifying the assigned TurnPoint.
  QString code;

  /// Name of the assigned TurnPoint.
  QString name;

  /// Geographical location of the task point
  Util::Location location;

  /// structure defining shape of the assigned area.
  AssignedArea assignedArea;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKPOINT_H_
