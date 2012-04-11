#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_

#include <QtGui/QMainWindow>
#include <QHash>
#include <QString>
// #include "ui_qtgui.h"

class QPushButton;
class QButtonGroup;

namespace Ui { class TaskDeclPanel; }

namespace Updraft {

class TaskLayer;
class TaskFile;

class TaskDeclPanel : public QWidget {
  Q_OBJECT

 public:
  TaskDeclPanel(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~TaskDeclPanel();

  void setTaskLayer(TaskLayer* layer) { taskLayer = layer; }

  /// \return Whether an add turnpoint button is toggled.
  bool hasToggledButton();

  /// \return Index of the turnpoint that should be added for the toggled btn.
  int getToggledButtonIndex();

  /// Create and insert into GUI a new turnpoint button
  void newTurnpointButton(int index, const QString& name);

  /// Create and insert into GUI a new add turnpoint button
  void newAddTpButton(int index, bool checked = false);

  /// Initializes the task from a file
  void initFromFile(TaskFile* file);

 private slots:
  void addTpButtonPushed();
  void removeTpButtonPushed();
  void saveButtonPushed();
  void undoButtonPushed();
  void redoButtonPushed();
  void dataChanged();

 private:
  QString addTpText;  // This is here because of translations
  /// Hides or shows the add turnpoint button text according to button number
  void adjustAddTpText();

  Ui::TaskDeclPanel *ui;

  /// Button group that ensures just one checked add tp button
  QButtonGroup* addButtons;

  TaskLayer* taskLayer;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_TASKDECLPANEL_H_
