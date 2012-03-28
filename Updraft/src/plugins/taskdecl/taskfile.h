#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKFILE_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKFILE_H_

#include <QString>

namespace Updraft {

class TaskFile {
 public:
  /// State of task storage (on disk)
  enum StorageState {
    UNSTORED_EMPTY,
    UNSTORED_EDITED,
    STORED_SYNCHRONIZED,
    STORED_DIRTY
  };

  TaskFile();

  /// \return File name
  QString getFileName() const;

  /// \return Absolute (full) file path
  QString getFilePath() const;

  /// \return State of file storage
  StorageState getStorageState() const;

 private:
  QString filePath;
  StorageState storageState;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKFILE_H_
