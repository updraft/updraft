#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKAXIS_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKAXIS_H_

#include <QWidget>

namespace Updraft {

class TaskFile;
class TaskDeclPanel;

/// Graphical representation of task leg distances.
class TaskAxis : public QWidget {
  Q_OBJECT

 public:
  TaskAxis(const TaskDeclPanel *panel, const TaskFile *taskFile_);

 public slots:
  void taskDataChanged();

 private:
  const TaskFile *taskFile;
  const TaskDeclPanel *taskDeclPanel;

 protected:
  void paintEvent(QPaintEvent *e);
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKAXIS_H_
