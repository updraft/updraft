#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDATA_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDATA_H_

#include <QString>

namespace Updraft {

class DataHistory;

class TaskData {
 private:
  /// Only DataHistory objects can create TaskData.
  TaskData();

  /// Only DataHistory objects can make copies of TaskData.
  TaskData(const TaskData&);

  friend class DataHistory;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDATA_H_
