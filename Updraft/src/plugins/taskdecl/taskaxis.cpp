#include "taskaxis.h"
#include "taskfile.h"
#include "taskpoint.h"

namespace Updraft {

TaskAxis::TaskAxis(const TaskFile *taskFile_)
  : taskFile(taskFile_) {
}

void TaskAxis::taskDataChanged() {
  if (taskFile == NULL) {
    return;
  }

  /* const TaskData *taskData = taskFile->beginRead();
  int pointIndex = 0;
  const TaskPoint *point1 = NULL, *point2 = NULL;
  while (point1 = taskData->getTaskPoint(pointIndex)
    && point2 = taskData->getTaskPoint(pointIndex+1)) {
    // distance(point1->getLocation(), point2->getLocation);

    ++pointIndex;
  }*/
  taskFile->endRead();
}

}  // End namespace Updraft
