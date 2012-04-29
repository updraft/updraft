#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKAXIS_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKAXIS_H_

#include <QWidget>

namespace Updraft {

class TaskFile;

class TaskAxis : public QWidget {
  Q_OBJECT

 public:
  explicit TaskAxis(const TaskFile *taskFile_);

 public slots:
  void taskDataChanged();

 private:
  const TaskFile *taskFile;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKAXIS_H_
