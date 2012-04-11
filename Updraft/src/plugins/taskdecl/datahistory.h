#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_DATAHISTORY_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_DATAHISTORY_H_

#include <QList>

namespace Updraft {

class TaskData;

/// This class creates and stores TaskData objects.
/// It manages undo and redo actions.
/// For proper editing first call add() to create
/// copy of the current item, then call getCurrent() and do changes.
class DataHistory {
 public:
  /// Creates DataHistory object with one default data entry.
  DataHistory();

  virtual ~DataHistory();

  /// \return Current item
  TaskData* getCurrent();

  /// \return Current item
  const TaskData* getCurrent() const;

  /// \return true if the current item is the first in the DataHistory
  bool isFirst() const;

  /// \return true if the current item is the last in the DataHistory
  bool isLast() const;

  /// \return true if the current item is marked
  bool isMarked() const;

  /// Ascends to the next item in the DataHistory.
  /// \return true if the current iterator has been moved.
  bool moveForward();

  /// Steps back to the previous item in the DataHistory.
  /// \return true if the current iterator has been moved.
  bool moveBack();

  /// Removes all newer items than current.
  /// Creates copy of the current item, appends it to the DataHistory
  /// and sets it as a new current item.
  void storeState();

  /// Set mark to the current item. Only one item at once is marked.
  void setMark();

 private:
  typedef QList<TaskData*> DataContainer;
  typedef DataContainer::iterator DataIterator;

  /// Container storing data history.
  DataContainer dataContainer;

  /// Iterator to current item in dataContainer.
  DataIterator currentItem;

  /// Iterator to marked item in dataContainer.
  DataIterator markedItem;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_DATAHISTORY_H_
