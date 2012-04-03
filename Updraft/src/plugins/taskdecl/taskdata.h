#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDATA_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDATA_H_

#include <QVector>

namespace Updraft {

class DataHistory;
class TaskPoint;

/// This class encapsulates all data related to one task.
/// It can be used only with DataHistory and TaskFile.
/// These three classes together form a Memento design pattern.
class TaskData {
 public:
  virtual ~TaskData();

  /// \return Xml text representation of TaskData
  QString toXml() const;

  /// Find and return TaskPoint on specified position.
  /// This function serves for both reading and editing of TaskPoint.
  /// \param position position of TaskPoint in sequence
  /// \return Pointer to a task point on desired position
  /// If position is out of range, it returns NULL.
  TaskPoint* getTaskPoint(int position);

  /// Tries to parse xml file. If it fails, TaskData is empty.
  /// \param serialized string with xml file content
  /// \return True on success, otherwise false.
  bool fromXml(const QString &serialized);

  /// Inserts new TaskPoint. All TaskPoints are deleted in ~TaskData.
  /// \param position target position of TaskPoint in sequence
  /// If position is invalid, TaskPoint is appended to end of sequence.
  void insertTaskPoint(TaskPoint *taskPoint, int position);

  /// Changes position of TaskPoint in sequence.
  /// \param from position of desired TaskPoint
  /// \param to destination position
  /// If "to" is invalid position, TaskPoint is moved to end of sequence.
  void moveTaskPoint(int from, int to);

  /// Deletes TaskPoint on desired position
  /// \param position position of target TaskPoint
  void deleteTaskPoint(int position);

 private:
  /// Only DataHistory objects can create TaskData.
  TaskData();

  /// Only DataHistory objects can make copies of TaskData.
  TaskData(const TaskData& taskData);

  typedef QVector<TaskPoint*> PointsContainer;

  /// Ordered sequence of TaskPoints
  PointsContainer taskPoints;

  friend class DataHistory;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDATA_H_
