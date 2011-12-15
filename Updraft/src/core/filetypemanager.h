#ifndef UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_
#define UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

#include <QMultiMap>
#include <QString>

#include "../coreinterface.h"

namespace Updraft {

class IPlugin;

namespace Core {

struct FileType {
  QString extension;
  QString description;
  FileCategory category;
};

class FileTypeManager {
 public:
  /// This plugin knows how to open the file!
  /// \return Identifier of the newly registered filetype
  FileType* registerFiletype(QString extension, QString description,
    FileCategory category);

  /// Unregisters a given file type
  void unregisterFileType(FileType *t);
 private:
  QVector<FileType*> registered;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

