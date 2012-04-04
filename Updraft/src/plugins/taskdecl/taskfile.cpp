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
}

}  // End namespace Updraft
