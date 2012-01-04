#include "settingsdialog.h"
#include "ui_settingsdialog.h"

namespace Updraft {
namespace Core {

SettingsDialog::SettingsDialog(QWidget* parent)
: QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("Settings"));

  connect(ui->topView, SIGNAL(clicked(const QModelIndex&)), ui->bottomView, SLOT(setTopIndex(const QModelIndex&)));
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::setModel(QStandardItemModel* model) {
  ui->topView->setModel(model);
  ui->topView->resize(sizeHint());
  ui->bottomView->setModel(model);
  ui->bottomView->resize(sizeHint());
}

}  // End namespace Core
}  // End namespace Updraft

