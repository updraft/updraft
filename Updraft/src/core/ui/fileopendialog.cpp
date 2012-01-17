#include "fileopendialog.h"

namespace Updraft {
namespace Core {

/// Add the preview list to the dialog.
/// \note This class uses dark magic and eats babies.
///   Also it depends on the implementation of QFileDialog, which may later
///   change. We are forced to do this because there is no clean way to
///   add preview in current versions of Qt.There are some checks to avoid
///   completescrew-ups though.
/// \param caption Title of the dialog window.
FileOpenDialog::FileOpenDialog(QWidget* parent, QString caption)
  : QFileDialog(parent, caption) {
  setFileMode(QFileDialog::ExistingFile);
  setNameFilters(getFilters());

  QSplitter* splitter = findChild<QSplitter*>("splitter");

  if (!splitter) {
    qDebug() << "It looks like QFileDialog changed. This is a bug.";
    havePreview = false;
    return;
  }

  havePreview = true;

  QListView* listView = new QListView(this);
  listView->setModel(&model);

  splitter->addWidget(listView);

  connect(this, SIGNAL(currentChanged(const QString&)),
    this, SLOT(changed(const QString)));
}

/// Returns a list of file name filters suitable
/// for QFileDialog::setNameFilters().
QStringList FileOpenDialog::getFilters() const {
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

/// Show the dialog and use it to open the files.
void FileOpenDialog::openIt() {
  if (!exec()) {
    return;
  }

  foreach(QString file, selectedFiles()) {
    qDebug() << file;
    if (havePreview) {
      updraft->fileTypeManager->openFileInternal(file, &model);
    } else {
      updraft->fileTypeManager->openFile(file, true);
    }
  }
}

/// A file was selectecte in the dialog.
void FileOpenDialog::changed(const QString path) {
  updraft->fileTypeManager->getOpenOptions(path, &model);
}

}  // End namespace Core
}  // End namespace Updraft

