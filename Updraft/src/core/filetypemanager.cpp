#include "filetypemanager.h"

#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>

#include "../pluginbase.h"
#include "ui/fileopendialog.h"
#include "ui/filerolesdialog.h"
#include "updraft.h"

namespace Updraft {
namespace Core {

/// Represents a single option to open a file.
class FileTypeManager::FileOpenOption : public QStandardItem {
 public:
  explicit FileOpenOption(const FileRegistration &r);

  /// QStandardItem requires this.
  int type() const {
    return UserType;
  }

  /// Should this option be used?
  bool selected() const {
    return checkState() == Qt::Checked;
  }

  FileRegistration registration;
};

FileTypeManager::FileOpenOption::FileOpenOption(const FileRegistration &r)
  : QStandardItem(r.roleDescription), registration(r) {
  setCheckable(true);
  setCheckState(Qt::Checked);
}

/// Register a file that can be opened by a plugin.
/// One file can be opened in many ways. Each registration describes one way.
void FileTypeManager::registerFiletype(const FileRegistration &registration) {
  registered.append(registration);
}

/// Open a file at given path.
/// \param path Path to the file to open.
/// \param showDialog If this is false, then all found options for opeing the
///   file are used and no gui elements are displayed.
/// \return true if opening was successful.
bool FileTypeManager::openFile(const QString &path, bool showDialog) const {
  QStandardItemModel model;

  qDebug() << "Opening file " << path << ".";
  getOpenOptions(path, &model);

  if (!model.rowCount()) {
    qDebug() << "No plugin can open this file.";
    return false;
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
/// \param [out] model Model that contains the options for opening the file.
///   Only items that are checked are
/// \param model Model with FileOpenOption instances. This is a little fragile
///   -- if the model contains something else than FileOpenOption bad things
///   will happen.
/// \return true if opening was successful.
bool FileTypeManager::openFileInternal(const QString &path,
  QStandardItemModel const* model) const {
  bool success = true;

  for (int i = 0; i < model->rowCount(); ++i) {
    FileOpenOption* option = static_cast<FileOpenOption*>(model->item(i));

    if (option->selected()) {
      const FileRegistration &registration = option->registration;

      // Path is changed for imported files.
      QString changedPath(path);

      // Import persistent file.
      if (registration.category == CATEGORY_PERSISTENT) {
        if (!importFile(&changedPath, registration.importDirectory, path)) {
          success = false;
        }
      }

      if (!registration.plugin->fileOpen(changedPath, registration.roleId)) {
        qDebug() << "Plugin "
          << registration.plugin->getName() << " failed to open file.";
        success = false;
      }
    }
  }

  return success;
}

/// Copy a file to import directory
/// \param [out] newPath contains destination path, if the function
///   is successful This parameter is optional.
/// \param importDirectory a target subdirectory of application data dir.
///   If importDirectory is empty, file will be copied
///   to application data directory.
/// \param srcPath a path to file which is to be imported.
/// \return True on success, otherwise returns false.
bool FileTypeManager::importFile(QString *newPath,
  const QString &importDirectory, const QString &srcPath) const {
  QString dstPath = updraft->getDataDirectory();

  if (importDirectory.length() != 0) {
    dstPath += "/" + importDirectory;
  }

  // Create whole branch of directories specified by path.
  QDir().mkpath(dstPath);

  if (!QDir(dstPath).exists()) {
    qDebug() << "Import failed. Unable to create directory " << dstPath;
    return false;
  }

  QFileInfo srcInfo(srcPath);

  if (!srcInfo.isFile()) {
    qDebug() << "Import failed. " << srcPath << "is not a file.";
    return false;
  }

  dstPath += "/" + srcInfo.fileName();

  QFileInfo dstInfo(dstPath);

  // Rename file when exists by appending index.
  // TODO(Tom): Query for replace/rename/cancel
  int i = 1;
  while (dstInfo.isFile()) {
    dstInfo = QFileInfo(dstInfo.absolutePath() + "/" + srcInfo.baseName()
      + "_" + QString("%1").arg(i, 2, 10, QChar('0'))
      + "." + srcInfo.completeSuffix());
    ++i;
  }

  if (!QFile::copy(srcPath, dstInfo.filePath())) {
    qDebug() << "Import failed. Unable to copy file from "
      << srcPath << " to " << dstInfo.filePath();
    return false;
  }

  qDebug() << "File imported to " << dstInfo.filePath();

  if (newPath != NULL)
    *newPath = dstInfo.filePath();
  return true;
}

/// Fill the output model with possible ways of opening this file.
/// \param path File to open.
/// \param [out] model Model that will contain the options.
///   The model is cleared before, and each item is initially selected.
void FileTypeManager::getOpenOptions(QString path,
  QStandardItemModel* model) const {
  model->clear();

  foreach(FileRegistration registration, registered) {
    if (path.endsWith(registration.extension, Qt::CaseInsensitive)) {
      model->appendRow(new FileOpenOption(registration));
    }
  }
}

/// Display a file open dialog, and open the selected files.
/// \param caption Title of the file open dialog.
/// \note This method uses dark magic and eats babies.
/// Also it depends on the implementation of QFileDialog, which may later
/// change, because ther is no clean way to do the required functionality
/// in current versions of Qt. We're doing some checks to avoid total
/// screw-ups though.
void FileTypeManager::openFileDialog(const QString &caption) const {
  FileOpenDialog dialog(updraft->mainWindow, caption);
  dialog.openIt();
}

}  // End namespace Core
}  // End namespace Updraft

