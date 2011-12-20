#include "filetypemanager.h"

#include <QDebug>
#include <QStandardItemModel>

#include "../pluginbase.h"
#include "ui/filerolesdialog.h"

namespace Updraft {
namespace Core {

/// Represents a single option to open a file.
class FileOpenOption : public QStandardItem {
 public:
  FileOpenOption(PluginBase *plugin_, int role_, QString text);

  /// QStandardItem requires this.
  int type() const {
    return UserType;
  }

  PluginBase* plugin;
  int role;
};


FileOpenOption::FileOpenOption(PluginBase *plugin_, int role_, QString text)
  : QStandardItem(text), plugin(plugin_), role(role_) {
  setCheckable(true);
  setCheckState(Qt::Checked);
}

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
/// \param showDialog If this is false, then all found options for opeing the
///   file are used and no gui elements are displayed.
/// \return true if opening was successful.
bool FileTypeManager::openFile(QString path, FileCategory category,
  bool showDialog) {
  QStandardItemModel options;

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
        options.appendRow(new FileOpenOption(type.plugin, i, roles[i]));
      }
    }
  }

  if (!options.rowCount()) {
    qDebug() << "No plugin can open this file (" << identificationsTried <<
      " identifications tried.)";

    return false;
  }

  if (showDialog) {
    FileRolesDialog dlg(NULL);  // TODO(Kuba): set the main window as parent.
    dlg.setList(&options);
    if (!dlg.exec()) {
      qDebug() << "Open was canceled.";
      return false;
    }
  }

  bool success = true;

  QList<int> roles;
  PluginBase* prevPlugin = NULL;

  for (int i = 0; i < options.rowCount(); ++i) {
    FileOpenOption* option = static_cast<FileOpenOption*>(options.item(i));
    if (option->checkState() != Qt::Checked) {
      continue;
    }

    if (prevPlugin == NULL) {
      prevPlugin = option->plugin;
    } else if (prevPlugin != option->plugin) {
      bool ret = option->plugin->fileOpen(path, roles);
      if (!ret) {
        qDebug() << "Plugin " << option->plugin->getName() <<
          " failed to open file.";
        success = false;
      }

      roles.clear();
      prevPlugin = option->plugin;
    }
    roles.append(option->role);
  }

  bool ret = prevPlugin->fileOpen(path, roles);
  if (!ret) {
    qDebug() << "Plugin " << prevPlugin->getName() <<
      " failed to open file.";
    success = false;
  }

  return success;
}

/// Returns a list of file name filters suitable
/// for QFileDialog::setNameFilters().
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
QStringList getFilters(FileCategory category = CATEGORY_ANY) {
  QStringList ret;

  foreach(FileType type, registered) {
    if (!(category & type.category)) {
      continue;
    }

    ret.append(type->description + " (*" + type->extension + ")");
  }

  return ret;
}

}  // End namespace Core
}  // End namespace Updraft

