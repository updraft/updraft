#ifndef UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_
#define UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

#include <QList>
#include <QString>

#include "../coreinterface.h"

namespace Updraft {

class PluginBase;

namespace Core {

struct FileType {
  QString extension;
  QString description;
  FileCategory category;
  PluginBase *plugin;
};

/// Handles file types, opening of files.
/// Dispatches file opens to plugins.
class FileTypeManager {
 public:
  void registerFiletype(const QString extension, const QString description,
    FileCategory category, PluginBase* plugin);

  bool openFile(const QString path, FileCategory category = CATEGORY_ANY);

 private:
  QList<FileType> registered;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

