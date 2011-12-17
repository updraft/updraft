#ifndef UPDRAFT_SRC_CORE_UI_FILEROLESDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_FILEROLESDIALOG_H_

#include <QDialog>

namespace Ui { class FileRolesDialog; }

class FileRolesDialog : public QDialog {
  Q_OBJECT

 public:
  explicit FileRolesDialog(QWidget *parent = 0);
  ~FileRolesDialog();

 private:
  Ui::FileRolesDialog *ui;
};

#endif  // UPDRAFT_SRC_CORE_UI_FILEROLESDIALOG_H_
