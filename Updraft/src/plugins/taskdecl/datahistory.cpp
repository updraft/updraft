#include "datahistory.h"
#include "taskdata.h"

namespace Updraft {

DataHistory::DataHistory()
  : currentItem(dataContainer.end()), markedItem(dataContainer.end()) {
}

DataHistory::~DataHistory() {
  clear();
}

TaskData* DataHistory::getCurrent() {
  if (isEmpty())
    add();

  return *currentItem;
}

bool DataHistory::isEmpty() const {
  return dataContainer.empty();
}

bool DataHistory::isFirst() const {
  return !isEmpty() && currentItem == dataContainer.begin();
}

bool DataHistory::isLast() const {
  return !isEmpty() && currentItem == --(dataContainer.end());
}

bool DataHistory::isMarked() const {
  return !isEmpty() && markedItem == currentItem;
}

void DataHistory::moveForward() {
  if (!isEmpty() && !isLast())
    ++currentItem;
}

void DataHistory::moveBack() {
  if (!isEmpty() && !isFirst())
    --currentItem;
}

void DataHistory::add() {
  // Erase all newer item than current.
  if (!isEmpty() && !isLast()) {
    DataIterator nextItem = currentItem;
    dataContainer.erase(++nextItem, dataContainer.end());
  }

  // Append copy of current item.
  dataContainer.push_back(new TaskData(**currentItem));

  // Ascend current item iterator to new item.
  ++currentItem;
}

void DataHistory::setMark() {
  if (!isEmpty())
    markedItem = currentItem;
}

void DataHistory::clear() {
  for (DataIterator it = dataContainer.begin();
    it != dataContainer.end(); ++it) {
    delete *it;
    *it = NULL;
  }

  dataContainer.clear();
  currentItem = dataContainer.end();
  markedItem = dataContainer.end();
}

}  // End namespace Updraft
