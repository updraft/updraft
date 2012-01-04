#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_

#include <QtGui>
#include "settingstopview.h"

namespace Ui { class SettingsDialog; }

namespace Updraft {
namespace Core {

class SettingsDialog: public QDialog {
  Q_OBJECT

 public:
  SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();

  void setModel(QStandardItemModel* model);

  Ui::SettingsDialog* ui;
 private:
  SettingsTopView* topView;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_

