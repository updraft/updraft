#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_

#include <QtGui>
#include "settingstopview.h"

namespace Ui { class SettingsDialog; }

namespace Updraft {
namespace Core {

class SettingsDelegate;

class SettingsDialog: public QDialog {
  Q_OBJECT

 public:
  SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

  void setModel(QStandardItemModel* model);

  void recalculateTopViewWidth();

  /// Commits the data of the bottom view editors to the model.
  void commitEditorData();

  /// Reverts data of the bottom view editors to values present in the model.
  void resetEditors();

 protected slots:
  void buttonBoxClicked(QAbstractButton* button);

 protected:
  Ui::SettingsDialog* ui;

  SettingsDelegate* settingsDelegate;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_

