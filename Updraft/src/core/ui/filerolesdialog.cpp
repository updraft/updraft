#include "filerolesdialog.h"
#include "ui_filerolesdialog.h"

FileRolesDialog::FileRolesDialog(QWidget *parent)
  : QDialog(parent), ui(new Ui::FileRolesDialog) {
  ui->setupUi(this);
}

FileRolesDialog::~FileRolesDialog() {
  delete ui;
}

void FileRolesDialog::setList(QStandardItemModel *model) {
  ui->listView->setModel(model);
}
