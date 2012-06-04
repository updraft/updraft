#ifndef UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_

#include <QFileDialog>
#include <QStringList>
#include <QStandardItemModel>

#include "../updraft.h"

namespace Updraft {
namespace Core {

/// Customized version of file open dialog that
/// has pre-filled filters and mode.
class FileOpenDialog : public QFileDialog {
  Q_OBJECT
 public:
  /// Display a file open dialog, and open the selected files.
  /// \param caption Title of the file open dialog.
  /// \param dir Directory first shown in the dialog.
  /// \return First selected file. This is only used to retrieve the dir
  ///   for the next appearance of the file dialog.
  static QString openIt(const QString& caption, const QString& dir = QString());

 private:
  /// Returns a list of file name filters suitable
  /// for QFileDialog::setNameFilters().
  static QStringList getFilters();
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_

