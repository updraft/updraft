#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_

#include <QtGui/QMainWindow>
// #include "ui_qtgui.h"

namespace Ui { class TaskDeclPanel; }

class TaskDeclPanel : public QWidget {
  Q_OBJECT

 public:
  TaskDeclPanel(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~TaskDeclPanel();

 private:
  Ui::TaskDeclPanel *ui;
};

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_
