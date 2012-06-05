#ifndef UPDRAFT_SRC_CORE_UI_ABOUTDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_ABOUTDIALOG_H_

#include <QtGui>
#include "ui_aboutdialog.h"

namespace Updraft {
namespace Core {

class AboutDialog: public QDialog {
  Q_OBJECT

 public:
  AboutDialog(QWidget* parent, QString version);
  ~AboutDialog();

 protected:
  Ui::AboutDialog* ui;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_ABOUTDIALOG_H_

