#include "aboutdialog.h"
#include "ui_aboutdialog.h"

namespace Updraft {
namespace Core {

AboutDialog::AboutDialog(QWidget* parent, QString version)
: QDialog(parent),
  ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  ui->versionLabel->setText(tr("Version: ") + version);
}

AboutDialog::~AboutDialog() {
  delete ui;
}

}  // End namespace Core
}  // End namespace Updraft

