#ifndef UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_
#define UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

#include <QCoreApplication>  // For Q_DECLARE_TR_FUNCTIONS
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

  bool openFile(const QString path, FileCategory category,
    bool showDialog = true) const;

  void openFileDialog(FileCategory category, QString caption) const;

 private:
  Q_DECLARE_TR_FUNCTIONS(FileTypeManager);  // Creates context for translations

  class FileOpenOption;
  class FileOpenOptionModel;

  void getOpenOptions(QString path, FileCategory category,
    FileOpenOptionModel* out) const;
  QStringList getFilters(FileCategory category) const;

  bool openFileInternal(const QString path,
    FileOpenOptionModel const* model) const;
  bool openFileOnePlugin(PluginBase* plugin,
    const QString path, QList<int> roles) const;

  QList<FileType> registered;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_FILETYPEMANAGER_H_

