#include "fileopendialog.h"
#include "ui/mainwindow.h"
#include "filetypemanager.h"

namespace Updraft {
namespace Core {

/// Returns a list of file name filters suitable
/// for QFileDialog::setNameFilters().
QStringList FileOpenDialog::getFilters() {
  typedef QMap<QString, QString> TMap;
  TMap mapFilters;

  // Create map of extensions (without duplicities).
  foreach(FileRegistration fileReg, updraft->fileTypeManager->registered) {
    mapFilters.insert(fileReg.extension, "");
  }

  // Assign descriptions to extensions.
  foreach(FileRegistration fileReg, updraft->fileTypeManager->registered) {
    if (fileReg.typeDescription.length() == 0) {
      continue;
    }

    TMap::iterator itFilter = mapFilters.find(fileReg.extension);
    if (itFilter == mapFilters.end()) {
      continue;
    }

    // Concatenate descriptions with same extensions.
    if (itFilter->length() == 0) {
      itFilter.value() = fileReg.typeDescription;
    } else {
      itFilter->append(" | " + fileReg.typeDescription);
    }
  }

  QString allExtensions;
  QStringList ret;

  // Insert filters to output list, create "all filter"
  for (TMap::iterator itFilter = mapFilters.begin();
    itFilter != mapFilters.end(); ++itFilter) {
    QString ext("*" + itFilter.key());

    if (allExtensions.length() == 0) {
      allExtensions = ext;
    } else {
      allExtensions.append(" " + ext);
    }

    ret.append((*itFilter) + " (" + ext + ")");
  }

  ret.prepend(tr("All supported types") + " (" + allExtensions + ")");
  return ret;
}

/// Display a file open dialog, and open the selected files.
/// \param caption Title of the file open dialog.
void FileOpenDialog::openIt(const QString& caption) {
  QStringList files;

  files = QFileDialog::getOpenFileNames(
    updraft->mainWindow, caption,
    QString(), getFilters().join(";;"));

  foreach(QString file, files) {
    updraft->fileTypeManager->openFile(file, true);
  }
}

}  // End namespace Core
}  // End namespace Updraft

