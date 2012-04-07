#include "datahistory.h"
#include "taskdata.h"

namespace Updraft {

DataHistory::DataHistory() {
  dataContainer.append(new TaskData());
  currentItem = dataContainer.begin();
  markedItem = dataContainer.end();
}

DataHistory::~DataHistory() {
  foreach(TaskData *data, dataContainer) {
    delete data;
  }
}

TaskData* DataHistory::getCurrent() {
  return *currentItem;
}

bool DataHistory::isFirst() const {
  return currentItem == dataContainer.begin();
}

bool DataHistory::isLast() const {
  return currentItem == --(dataContainer.end());
}

bool DataHistory::isMarked() const {
  return markedItem == currentItem;
}

bool DataHistory::moveForward() {
  if (!isLast()) {
    ++currentItem;
    return true;
  }

  return false;
}

bool DataHistory::moveBack() {
  if (!isFirst()) {
    --currentItem;
    return true;
  }

  return false;
}

void DataHistory::storeState() {
  // Erase all newer items than current.
  if (!isLast()) {
    DataIterator nextItem = currentItem;
    dataContainer.erase(++nextItem, dataContainer.end());
  }

  // Append copy of current item.
  dataContainer.push_back(new TaskData(**currentItem));

  // Ascend current item iterator to new item.
  ++currentItem;
}

void DataHistory::setMark() {
  markedItem = currentItem;
}

}  // End namespace Updraft
