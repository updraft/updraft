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
  static void openIt(const QString &caption);

 private:
  static QStringList getFilters();
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_

