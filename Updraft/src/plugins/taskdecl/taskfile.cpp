#include <QFileInfo>
#include "taskfile.h"
#include "taskdata.h"

namespace Updraft {

TaskFile::TaskFile()
  : storageState(UNSTORED_EMPTY), editing(false) {
}

TaskFile::TaskFile(const QString &filePath_)
  : storageState(UNSTORED_EMPTY), editing(false) {
  // Tries to open a file.
  QFile file(filePath_);
  if (!file.open(QIODevice::ReadOnly))
    return;

  // Loads from file.
  QString serialized(QByteArray(file.readAll()));
  file.close();

  // Parses xml. In case of success updates storageState.
  if (dataHistory.getCurrent()->fromXml(serialized)) {
    filePath = filePath_;
    storageState = STORED_SYNCHRONIZED;
  }
}

QString TaskFile::getFileName() const {
  return QFileInfo(filePath).fileName();
}

QString TaskFile::getFilePath() const {
  return filePath;
}

TaskFile::StorageState TaskFile::getStorageState() const {
  return storageState;
}

void TaskFile::save() {
  saveAs(filePath);
}

void TaskFile::saveAs(const QString &filePath_) {
  QFile file(filePath_);
  if (!file.open(QIODevice::WriteOnly))
    return;

  // Serializes TaskData in Xml format
  QString serialized = dataHistory.getCurrent()->toXml();

  // Saves TaskData to disk
  bool saved = true;
  if (file.write(serialized.toUtf8()) != (qint64)serialized.length()) {
    saved = false;
    qDebug("TaskFile: file.write() FAILED");
  }

  file.close();

  // Sets save mark and updates storage state
  if (saved) {
    dataHistory.setMark();
    storageState = STORED_SYNCHRONIZED;
    filePath = filePath_;

    // Emits signal announcing state changes.
    emit storageStateChanged();
  }
}

TaskData* TaskFile::beginEdit() {
  if (editing) {
    return NULL;
  }

  // Lock. This prevents next call of beginEdit() before endEdit().
  editing = true;

  return dataHistory.getCurrent();
}

void TaskFile::endEdit(bool storeState) {
  if (storeState) {
    dataHistory.storeState();
  }

  // Unlock for next editing session.
  editing = false;

  // Emits signal announcing data change.
  emit dataChanged();
}

const TaskData* TaskFile::getData() const {
  return dataHistory.getCurrent();
}

void TaskFile::undo() {
  if (dataHistory.moveBack()) {
    updateOnUndoRedo();
  }
}

void TaskFile::redo() {
  if (dataHistory.moveForward()) {
    updateOnUndoRedo();
  }
}

void TaskFile::updateOnUndoRedo() {
  // Emits signal announcing data change.
  emit dataChanged();

  // Updates storage state and emits signal announcing state changes.
  switch (storageState) {
    case UNSTORED_EMPTY:
      if (!dataHistory.isFirst()) {
        storageState = UNSTORED_EDITED;
        emit storageStateChanged();
      }
      break;

    case UNSTORED_EDITED:
      if (dataHistory.isFirst()) {
        storageState = UNSTORED_EMPTY;
        emit storageStateChanged();
      }
      break;

    case STORED_DIRTY:
      if (dataHistory.isMarked()) {
        storageState = STORED_SYNCHRONIZED;
        emit storageStateChanged();
      }
      break;

    case STORED_SYNCHRONIZED:
      if (!dataHistory.isMarked()) {
        storageState = STORED_DIRTY;
        emit storageStateChanged();
      }
      break;
  }
}

}  // End namespace Updraft
