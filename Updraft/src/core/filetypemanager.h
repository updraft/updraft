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
  void registerFiletype(const QString extension, const QString description,
    FileCategory category, PluginBase* plugin);

  bool openFile(const QString path, FileCategory category,
    bool showDialog = true) const;

  void openFileDialog(FileCategory category, QString caption) const;

 private:
  class FileOpenOption;

  struct FileType {
    QString extension;
    QString description;
    FileCategory category;
    PluginBase *plugin;
  };

  void getOpenOptions(QString path, FileCategory category,
    QStandardItemModel* out) const;

  bool openFileInternal(const QString path,
    QStandardItemModel const* model) const;
  bool openFileOnePlugin(PluginBase* plugin,
    const QString path, QList<int> roles) const;

  QList<FileType> registered;

  friend class FileOpenDialog;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

