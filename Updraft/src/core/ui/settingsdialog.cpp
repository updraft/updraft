#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settingsdelegate.h"

namespace Updraft {
namespace Core {

SettingsDialog::SettingsDialog(QWidget* parent)
: QDialog(parent), ui(new Ui::SettingsDialog), settingsDelegate(new SettingsDelegate) {
  ui->setupUi(this);

  ui->bottomView->setItemDelegate(settingsDelegate);

  setWindowTitle(tr("Settings"));

  connect(ui->topView, SIGNAL(clicked(const QModelIndex&)), ui->bottomView, SLOT(setTopIndex(const QModelIndex&)));
  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonBoxClicked(QAbstractButton*)));
}

SettingsDialog::~SettingsDialog() {
  delete settingsDelegate;
  delete ui;
}

void SettingsDialog::setModel(QStandardItemModel* model) {
  ui->topView->setModel(model);
  ui->topView->resize(sizeHint());
  ui->bottomView->setModel(model);
  ui->bottomView->resize(sizeHint());
}

void SettingsDialog::recalculateTopViewWidth() {
  int newColumnWidth = ui->topView->sizeHintForColumn(0);
  QSize maxSize = ui->topView->maximumSize();
  maxSize.setWidth(newColumnWidth + 10);   // Add some extra space
  ui->topView->setMaximumSize(maxSize);
}

void SettingsDialog::commitEditorData() {
  ui->bottomView->commit();
}

void SettingsDialog::resetEditors() {
  ui->bottomView->reset();
}

void SettingsDialog::buttonBoxClicked(QAbstractButton* button) {
  switch (ui->buttonBox->buttonRole(button)) {
    case QDialogButtonBox::ResetRole:
      resetEditors();
    break;
    case QDialogButtonBox::ApplyRole:
      commitEditorData();
    break;
    default:
    break;
  }
}

}  // End namespace Core
}  // End namespace Updraft

