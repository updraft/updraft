#include "filetypemanager.h"

#include <QDebug>

#include "../pluginbase.h"

namespace Updraft {
namespace Core {

struct IdentifyResult {
  PluginBase* plugin;
  int role;
  QString description;
};

/// Register a file extension that can be opened by a plugin.
/// \param extension File name extension with a dot (i.e. ".txt").
///   This value must match the end of file name.
/// \param description Human readable description for the file type.
///   Used in file open dialog.
/// \param category Binary or of category flags that this file type belongs to.
/// \param plugin Plugin that will be notified when opening this type.
void FileTypeManager::registerFiletype(QString extension, QString description,
  FileCategory category, PluginBase* plugin) {
  FileType type;
  type.extension = extension;
  type.description = description;
  type.category = category;
  type.plugin = plugin;

  registered.append(type);
}

/// Open a file at given path.
/// \param path Path to the file to open.
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
/// \return true if opening was successful.
bool FileTypeManager::openFile(QString path, FileCategory category) {
  QList<IdentifyResult> identifyResults;

  qDebug() << "Opening file " << path << ".";

  int identificationsTried = 0;

  foreach(FileType type, registered) {
    if (!(category & type.category)) {
      continue;
    }

    if (path.endsWith(type.extension)) {
      ++identificationsTried;
      QList<QString> roles = type.plugin->fileIdentification(path);

      for (int i = 0; i < roles.count(); ++i) {
        IdentifyResult r;
        r.plugin = type.plugin;
        r.role = i;
        r.description = roles[i];
        identifyResults.append(r);
      }
    }
  }

  if (!identifyResults.count()) {
    qDebug("No plugin can open this file (%i identifications tried.)",
      identificationsTried);

    return false;
  }

  // use the dialog to filter results

  foreach(IdentifyResult r, identifyResults) {
    // do something
  }
}

}  // End namespace Core
}  // End namespace Updraft

