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
class FileOpenDialog : public QFileDialog {
  Q_OBJECT
 public:
  FileOpenDialog(QWidget* parent, QString caption);

  void openIt();

 private slots:
  void changed(const QString path);

 private:
  QStringList getFilters() const;

  bool havePreview;
  QStandardItemModel model;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_FILEOPENDIALOG_H_

