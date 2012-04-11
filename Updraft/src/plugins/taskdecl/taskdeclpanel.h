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
class TaskPoint;

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

  /// Called when the buttons should be updated due to changes in the file
  void updateButtons();

 private:
  /// Says whether the turnpoint button exists for the given position.
  bool tpButtonExists(int pos);
  /// Says, whether the taskpoint button is a correct representation of
  /// the given TaskPoint.
  bool tpButtonCorrect(int pos, const TaskPoint* point);
  /// Updates the taskpoint button to represent the given TurnPoint.
  void updateTpButton(int pos, const TaskPoint* point);

  /// Removes TaskPoint buttons for the given position.
  void removeTpButtons(int pos);

  int tpIndexToLayoutPos(int index);
  int tpLayoutPosToIndex(int pos);
  int addIndexToLayoutPos(int index);
  int addLayoutPosToIndex(int pos);

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
