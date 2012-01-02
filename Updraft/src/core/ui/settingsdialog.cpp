#include "settingsdialog.h"
#include "ui_settingsdialog.h"

namespace Updraft {
namespace Core {

SettingsDialog::SettingsDialog(QWidget* parent, QStandardItemModel* model)
: QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("Settings"));

  ui->topView->setModel(model);
  ui->topView->resize(sizeHint());
  ui->bottomView->setModel(model);
  ui->bottomView->resize(sizeHint());

  connect(ui->topView, SIGNAL(clicked(const QModelIndex&)), ui->bottomView, SLOT(setTopIndex(const QModelIndex&)));
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

}  // End namespace Core
}  // End namespace Updraft

