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
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
FileOpenDialog::FileOpenDialog(QWidget* parent, QString caption,
  FileCategory category)
  : QFileDialog(parent, caption), categ(category) {
  setFileMode(QFileDialog::ExistingFile);
  setNameFilters(getFilters(category));

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
/// \param category Limit usable file types only to category.
///   If category contains more than one category flags any of the flags is
///   sufficient. CATEGORY_ALL will use all file types regardless of
///   category flags.
QStringList FileOpenDialog::getFilters(FileCategory category) const {
  QStringList ret;
  QStringList allFilters;

  foreach(FileTypeManager::FileType type,
    updraft->fileTypeManager->registered) {
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
      updraft->fileTypeManager->openFile(file, categ, true);
    }
  }
}

/// A file was selectecte in the dialog.
void FileOpenDialog::changed(const QString path) {
  updraft->fileTypeManager->getOpenOptions(path, categ, &model);
}

}  // End namespace Core
}  // End namespace Updraft

