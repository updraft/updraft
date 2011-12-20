#ifndef UPDRAFT_SRC_CORE_UI_FILEROLESDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_FILEROLESDIALOG_H_

#include <QDialog>
#include <QStandardItemModel>

namespace Ui { class FileRolesDialog; }

/// A dialog window used to select roles when opening a file.
class FileRolesDialog : public QDialog {
  Q_OBJECT

 public:
  explicit FileRolesDialog(QWidget *parent = 0);
  ~FileRolesDialog();

  void setList(QStandardItemModel *model);

 private:
  Ui::FileRolesDialog *ui;
};

#endif  // UPDRAFT_SRC_CORE_UI_FILEROLESDIALOG_H_
