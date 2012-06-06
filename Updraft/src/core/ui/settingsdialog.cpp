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
  ui->topView->setBottom(ui->bottomView);

  ui->restartNeededLabel->hide();

  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
    this, SLOT(buttonBoxClicked(QAbstractButton*)));
  connect(ui->showHidden, SIGNAL(stateChanged(int)),
    this, SLOT(hideCheckboxToggled()));
  connect(ui->bottomView, SIGNAL(restartInfo(bool)),
    this, SLOT(restartNeeded(bool)));
}

SettingsDialog::~SettingsDialog() {
  delete settingsDelegate;
  delete ui;
}

void SettingsDialog::setModel(QAbstractItemModel* model) {
  ui->topView->setModel(model);
  ui->topView->resize(sizeHint());
  ui->bottomView->setModel(model);
  ui->bottomView->resize(sizeHint());

  recalculateTopViewWidth();

  // simulate checkbox toggle to keepthe hidden settings hidden
  hideCheckboxToggled();
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

void SettingsDialog::hideCheckboxToggled() {
  bool state = ui->showHidden->checkState();
  bool showAdvanced = ui->topView->setShowAdvanced(state);
  ui->showHidden->setCheckState(showAdvanced ? Qt::Checked : Qt::Unchecked);
}

void SettingsDialog::restartNeeded(bool needed) {
  if (needed) {
    ui->restartNeededLabel->show();
  } else {
    ui->restartNeededLabel->hide();
  }
}

}  // End namespace Core
}  // End namespace Updraft

