#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settingsdelegate.h"
#include "../settingsmanager.h"

namespace Updraft {
namespace Core {

SettingsDialog::SettingsDialog(QWidget* parent, SettingsManager* manager)
: QDialog(parent),
  ui(new Ui::SettingsDialog),
  settingsDelegate(new SettingsDelegate),
  settingsManager(manager) {
  ui->setupUi(this);

  ui->bottomView->setItemDelegate(settingsDelegate);

  setWindowTitle(tr("Settings"));

  connect(ui->topView, SIGNAL(clicked(const QModelIndex&)),
    ui->bottomView, SLOT(setTopIndex(const QModelIndex&)));
  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
    this, SLOT(buttonBoxClicked(QAbstractButton*)));
  connect(ui->showHidden, SIGNAL(clicked()),
    ui->topView, SLOT(toggleShowHidden()));
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
  switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Reset:
      resetEditors();
    break;
    case QDialogButtonBox::Apply:
      commitEditorData();
    break;
    case QDialogButtonBox::RestoreDefaults:
      if (QMessageBox::warning(
        this,
        tr("Reset to defaults?"),
        tr("All settings values will be erased and set to defaults."
          " Are you sure you want to continue?"),
        QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok) {
        settingsManager->resetToDefaults();
      }
    break;
    default:
    break;
  }
}

}  // End namespace Core
}  // End namespace Updraft

