#ifndef UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_

#include <QFileDialog>
#include <QStringList>
#include <QStandardItemModel>

#include "../updraft.h"

namespace Updraft {
namespace Core {

/// Customized version of file open dialog that displays
/// preview / selection of roles for file opening,
/// has pre-filled filters and mode.
/// \note This class uses dark magic and eats babies.
///   Also it depends on the implementation of QFileDialog, which may later
///   change. We are forced to do this because there is no clean way to
///   add preview in current versions of Qt.There are some checks to avoid
///   completescrew-ups though.
class FileOpenDialog : public QFileDialog {
  Q_OBJECT
 public:
  FileOpenDialog(QWidget* parent, const QString& caption);

  /// Display a file open dialog, and open the selected files.
  /// \param caption Title of the file open dialog.
  static void openIt(const QString &caption);

 private slots:
  void changed(const QString path);

 private:
  static QStringList getFilters();

  bool havePreview;
  QStandardItemModel model;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_

