#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKFILE_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKFILE_H_

#include <QString>
#include "datahistory.h"

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

  /// Creates a new empty TaskFile.
  TaskFile();

  /// Creates a new TaskFile from specified file on a disk.
  /// If the file couldn't be loaded, empty task is created.
  /// \param filePath_ full path to a disk file
  explicit TaskFile(const QString &filePath_);

  /// \return File name
  QString getFileName() const;

  /// \return Absolute (full) file path
  QString getFilePath() const;

  /// \return State of file storage
  StorageState getStorageState() const;

  /// Save to current location.
  /// To obtain used path call getFilePath().
  void save();

  /// Save to new location.
  /// \param filePath_ new file path. Next call of save()
  /// will use this path.
  void saveAs(const QString &filePath_);

  /// Initialize editing session of current data.
  /// \return Current TaskData
  /// If other editing session is started, it returns NULL.
  TaskData* beginEdit();

  /// Ends session and optionally stores current state to history.
  /// \param storeState If it is true, current state is saved to history.
  void endEdit(bool storeState);

 private:
  /// Tries to load data from disk.
  /// \param filePath_ full path to a disk file
  bool load(const QString &filePath_);

  /// Full path to file in filesystem. Could be empty, if task is not saved.
  QString filePath;

  /// StorageState indicates whether file is saved on disk
  /// and if it has been changed since last save/load operation.
  StorageState storageState;

  /// DataHistory holds task editing history and also current data.
  DataHistory dataHistory;

  /// Flag indicating editing process.
  /// Set in beginEdit(), freed in endEdit().
  bool editing;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKFILE_H_
