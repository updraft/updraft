#ifndef UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_
#define UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

#include <QList>
#include <QString>
#include <QStandardItemModel>

#include "../coreinterface.h"

namespace Updraft {

class PluginBase;

namespace Core {

/// Handles file types, opening of files.
/// Dispatches file opens to plugins.
class FileTypeManager {
 public:
  void registerFiletype(const FileRegistration &registration);

  bool openFile(const QString &path, bool showDialog = true) const;

  void openFileDialog(const QString &caption) const;

 private:
  class FileOpenOption;

  void getOpenOptions(QString path, QStandardItemModel* out) const;

  bool importFile(QString *newPath,
    const QString &importDirectory, const QString &srcPath) const;

  bool openFileInternal(const QString &path,
    QStandardItemModel const* model) const;

  QList<FileRegistration> registered;

  friend class FileOpenDialog;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

