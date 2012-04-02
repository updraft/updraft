#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_

#include <QtGui/QMainWindow>
#include <QHash>
#include <QString>
// #include "ui_qtgui.h"

class QPushButton;

namespace Ui { class TaskDeclPanel; }

class TaskDeclPanel : public QWidget {
  Q_OBJECT

 public:
  TaskDeclPanel(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~TaskDeclPanel();

 private slots:
  void addTpButtonPushed();

 private:
  /// Create and insert into GUI a new turnpoint button
  void newTurnpointButton(int index);
  /// Create and insert into GUI a new add turnpoint button
  void newAddTpButton(int index);

  QString addTpText;  // This is here because of translations
  /// Hides or shows the add turnpoint button text according to button number
  void adjustAddTpText();

  Ui::TaskDeclPanel *ui;
};

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_
