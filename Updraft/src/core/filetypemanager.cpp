#include "filetypemanager.h"

#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>

#include "../pluginbase.h"
#include "ui/filerolesdialog.h"
#include "updraft.h"

namespace Updraft {
namespace Core {

/// Represents a single option to open a file.
class FileTypeManager::FileOpenOption : public QStandardItem {
 public:
  FileOpenOption(PluginBase *plugin_, int role_, QString text);

  /// QStandardItem requires this.
  int type() const {
    return UserType;
  }

  /// Should this option be used?
  bool selected() const {
    return checkState() == Qt::Checked;
  }

  PluginBase* plugin;
  int role;
};

/// Model for file open options. Wraps the two most needed methods, but don't
/// put anything else than FileItemOptions in!
class FileTypeManager::FileOpenOptionModel : public QStandardItemModel {
 public:
  void appendRow(FileOpenOption* item) {
    QStandardItemModel::appendRow(item);
  }

  FileOpenOption* item(int r, int c = 0) const {
    return static_cast<FileOpenOption*>(QStandardItemModel::item(r, c));
  }
};

FileTypeManager::FileOpenOption::FileOpenOption(
  PluginBase *plugin_, int role_, QString text)
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
  bool showDialog) const {
  FileOpenOptionModel model;

  qDebug() << "Opening file " << path << ".";
  getOpenOptions(path, category, &model);

  if (!model.rowCount()) {
    return false;  // This is already logged in getOpenOptions()
  }

  if (showDialog) {
    FileRolesDialog dlg(updraft->mainWindow);
    dlg.setList(&model);
    if (!dlg.exec()) {
      qDebug() << "Open was canceled.";
      return false;
    }
  }

  return openFileInternal(path, &model);
}

/// Open a file at given path once its open options
/// have been found and selected.
/// \param path Path to the file to open.
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
/// \param [out] model Model that contains the options for opening the file.
///   Only items that are checked are
/// \return true if opening was successful.
bool FileTypeManager::openFileInternal(QString path,
  FileOpenOptionModel const* model) const {
  bool success = true;

  QList<int> roles;
  PluginBase* prevPlugin = NULL;

  for (int i = 0; i < model->rowCount(); ++i) {
    FileOpenOption* option = model->item(i);
    if (!option->selected()) {
      continue;
    }

    if (prevPlugin && prevPlugin != option->plugin) {
      success = success && openFileOnePlugin(prevPlugin, path, roles);
      roles.clear();
    }
    prevPlugin = option->plugin;
    roles.append(option->role);
  }

  if (!prevPlugin) {
    return true;  // Opening the file with no roles is always successful.
  }

  success = success && openFileOnePlugin(prevPlugin, path, roles);

  return success;
}

/// Let a plugin open a file with a given set of roles.
bool FileTypeManager::openFileOnePlugin(PluginBase* plugin,
  const QString path, QList<int> roles) const {
  bool ret = plugin->fileOpen(path, roles);
  if (!ret) {
    qDebug() << "Plugin " << plugin->getName() << " failed to open file.";
  }
  return ret;
}

/// Fill the output model with possible ways of opening this file.
/// \param path File to open.
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
/// \param [out] model Model that will contain the options.
///   The model is cleared before, and each item is initially selected.
void FileTypeManager::getOpenOptions(QString path, FileCategory category,
  FileOpenOptionModel* model) const {
  int identificationsTried = 0;
  model->clear();

  foreach(FileType type, registered) {
    if (!(category & type.category)) {
      continue;
    }

    if (!path.endsWith(type.extension)) {
      continue;
    }

    ++identificationsTried;
    QList<QString> roles = type.plugin->fileIdentification(path);

    for (int i = 0; i < roles.count(); ++i) {
      model->appendRow(new FileOpenOption(type.plugin, i, roles[i]));
    }
  }

  if (!model->rowCount()) {
    qDebug() << "No plugin can open this file (" << identificationsTried <<
      " identifications tried.)";
  }
}

/// Returns a list of file name filters suitable
/// for QFileDialog::setNameFilters().
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
QStringList FileTypeManager::getFilters(FileCategory category) const {
  QStringList ret;
  QStringList allFilters;

  foreach(FileType type, registered) {
    if (!(category & type.category)) {
      continue;
    }

    QString filter = "*" + type.extension;

    allFilters.append(filter);
    ret.append(type.description + " (" + filter + ")");
  }

  allFilters.removeDuplicates();
  QString filter = allFilters.join(" ");
  ret.prepend(tr("All supported types") + " (" + filter + ")");

  return ret;
}

/// Display a file open dialog, and open the selected files.
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
void FileTypeManager::openFileDialog(FileCategory category,
QString caption) const {
  QFileDialog dialog(updraft->mainWindow, caption);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilters(updraft->fileTypeManager->getFilters(category));

  if (!dialog.exec()) {
    return;
  }

  QStringList list = dialog.selectedFiles();

  foreach(QString file, list) {
    openFile(file, category, true);
  }
}

}  // End namespace Core
}  // End namespace Updraft

