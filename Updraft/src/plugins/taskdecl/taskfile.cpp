#include <QFileInfo>
#include "taskfile.h"

namespace Updraft {

TaskFile::TaskFile()
  : storageState(UNSTORED_EMPTY) {
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

}  // End namespace Updraft
