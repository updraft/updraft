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
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

}  // End namespace Core
}  // End namespace Updraft

