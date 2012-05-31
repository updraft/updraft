#include <QDebug>
#include <QPushButton>
#include <QSize>
#include <QLayoutItem>
#include <QFileDialog>

#include "taskdeclpanel.h"
#include "ui_taskdeclpanel.h"
#include "tasklayer.h"
#include "taskfile.h"
#include "taskaxis.h"
#include "taskdata.h"
#include "taskpoint.h"
#include "taskpointbutton.h"

namespace Updraft {

TaskDeclPanel::TaskDeclPanel(TaskLayer* layer,
  QWidget *parent, Qt::WFlags flags)
  : QWidget(parent, flags),
  addTpText(tr("Add turnpoint")),
  ui(new Ui::TaskDeclPanel),
  taskLayer(layer) {
  // Create the UI
  ui->setupUi(this);
  addButtons = new QButtonGroup(this);
  addButtons->setExclusive(false);

  connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveButtonPushed()));
  connect(ui->undoButton, SIGNAL(clicked()), this, SLOT(undoButtonPushed()));
  connect(ui->redoButton, SIGNAL(clicked()), this, SLOT(redoButtonPushed()));

  taskAxis = new TaskAxis(this, taskLayer->getTaskFile());
  ui->gridLayout_3->addWidget(taskAxis);

  newAddTpButton(0);
}

TaskDeclPanel::~TaskDeclPanel() {
}

bool TaskDeclPanel::hasToggledButton() {
  return addButtons->checkedButton() != 0;
}

int TaskDeclPanel::getToggledButtonIndex() {
  if (addButtons->checkedButton() == NULL) {
    qDebug() << "TaskDeclPanel: There was no toggled button!";
    return -1;
  }

  int layoutIdx = ui->taskButtonsLayout->indexOf(addButtons->checkedButton());
  int buttIndex = addLayoutPosToIndex(layoutIdx);
  return buttIndex;
}

void TaskDeclPanel::addTpButtonPushed() {
  if (!sender()) {  // Sender might be NULL in some threaded scenarions...
    qDebug() << "sender() was NULL in a slot. See Qt documentation for info";
    return;
  }

  // Remember check state of the sender button
  QAbstractButton* senderButton = qobject_cast<QAbstractButton*>(sender());
  bool newCheckedState = senderButton->isChecked();

  // Uncheck all other addTp buttons
  uncheckAllAddTpButtons();

  // Toggle sender button and the editing state
  senderButton->setChecked(newCheckedState);
  isBeingEdited = senderButton->isChecked();

  // Set the toggled button in the task data
  TaskFile* file = taskLayer->getTaskFile();
  TaskData* data = file->beginEdit(false);
  int addButtonIndex = ui->taskButtonsLayout->indexOf(senderButton);
  addButtonIndex = addLayoutPosToIndex(addButtonIndex);
  data->setAddTaskPointButton(addButtonIndex);
  file->endEdit();
}

void TaskDeclPanel::removeTpButtonPushed() {
  if (!sender()) {  // Sender might be NULL in some threaded scenarions...
    qDebug() << "sender() was NULL in a slot. See Qt documentation for info";
    return;
  }

  QObject* closeFrame = sender()->parent();
  if (!closeFrame) {
    qDebug() << "could not get to closeFrame in removeTpButtonPushed!";
    return;
  }

  QObject* topFrameObj = closeFrame->parent();
  QFrame* topFrame = qobject_cast<QFrame*>(topFrameObj);
  if (!topFrame) {
    qDebug() << "could not get to topFrame in removeTpButtonPushed!";
    return;
  }

  // Let's remove the top frame and the plus button now
  TaskFile* file = taskLayer->getTaskFile();

  TaskData* data = file->beginEdit(true);
  int pos = ui->taskButtonsLayout->indexOf(topFrame);
  data->deleteTaskPoint(tpLayoutPosToIndex(pos));
  file->endEdit();
}

void TaskDeclPanel::saveButtonPushed() {
  taskLayer->save();
}

void TaskDeclPanel::undoButtonPushed() {
  taskLayer->undo();
}

void TaskDeclPanel::redoButtonPushed() {
  taskLayer->redo();
}

void TaskDeclPanel::updateButtons() {
  TaskFile* file = taskLayer->getTaskFile();

  if (!file) return;

  // First, we need to uncheck all buttons
  uncheckAllAddTpButtons();

  // Iterate over the task data and update the buttons
  const TaskData* data = file->beginRead();
  int pos = 0;
  while (const TaskPoint* point = data->getTaskPoint(pos)) {
    if (!tpButtonExists(pos)) {
      newTurnpointButton(pos, point->getName());
      newAddTpButton(pos+1, false);
    } else if (!tpButtonCorrect(pos, point)) {
      updateTpButton(pos, point);
    }
    pos++;
  }

  // Check the corresponding add button if in editing mode
  int checkedButton = data->getAddTaskPointButton();
  if (isBeingEdited && checkedButton != -1) {
    updateAddTpButton(checkedButton, true);
  }
  file->endRead();

  // Iterate over any remaining task buttons
  while (tpButtonExists(pos)) {
    removeTpButtons(pos);
  }
}

const QWidget* TaskDeclPanel::getTaskPointWidget(int i) const {
  int layoutPos = tpIndexToLayoutPos(i);
  QLayoutItem* item = ui->taskButtonsLayout->itemAt(layoutPos);
  if (!item) {
    return NULL;
  }

  return item->widget();
}

void TaskDeclPanel::dataChanged() {
  const TaskData* data = taskLayer->getTaskFile()->beginRead();
  updateSummaryLabel(data);
  taskLayer->getTaskFile()->endRead();

  // Enables/disables undo,redo buttons.
  ui->undoButton->setEnabled(!taskLayer->getTaskFile()->isFirstInHistory());
  ui->redoButton->setEnabled(!taskLayer->getTaskFile()->isLastInHistory());
}

void TaskDeclPanel::updateSummaryLabel(const TaskData* data) {
  QString text;

  bool closedCourse = data->isClosed();
  int count = data->size();
  if (closedCourse) {
    // in closed course we count start and landing as one point
    count -= 1;
  }

  if (data->isFaiTriangle()) {
    if (count == 3) {
      text = tr("FAI Triangle");
    } else {
      text = tr("FAI Triangle (4 task points)");
    }
  } else if (closedCourse) {
    if (count == 3) {
        text = tr("Triangle");
    } else if (count == 2) {
        text = tr("Out and Return");
    } else {
      text = tr("Closed course") + ", " + tr("%1 task points").arg(count);
    }
  } else {
    text = tr("%1 task points").arg(count);
  }

  text.append(" - ");

  qreal officialDistance = data->officialDistance();
  text.append(tr("%1 km").arg(officialDistance / 1000, 0, 'f', 1));

  qreal totalDistance = data->totalDistance();
  if (totalDistance != officialDistance) {
    text.append(tr(" (total %1 km)").arg(totalDistance / 1000, 0, 'f', 1));
  }

  ui->taskSummaryLabel->setText(text);
}

void TaskDeclPanel::newTurnpointButton(int index, const QString& name) {
  // Check the index argument
  if (index > ui->taskButtonsLayout->count() - 2) {  // 2 for the spacers
    qDebug() << "Wrong index parameter in TaskDeclPanel::newTurnpointButton: "
      << index;
    return;
  }

  TaskPointButton* button = new TaskPointButton(index, name);
  button->connectQuit(this, SLOT(removeTpButtonPushed()));

  // Insert the GUI into the panel GUI
  int layoutPos = tpIndexToLayoutPos(index);
  ui->taskButtonsLayout->insertWidget(layoutPos, button);
}

void TaskDeclPanel::newAddTpButton(int index, bool checked) {
  // Check the index argument
  if (index > ui->taskButtonsLayout->count() - 2) {  // 2 for the spacers
    qDebug() << "Wrong index parameter in TaskDeclPanel::newAddTaskButton: "
      << index;
    return;
  }

  // Create the button
  QPushButton* butt = new QPushButton(
    QIcon(":/taskdeclpanel/icons/add_tp_icon.png"), addTpText, NULL);
  butt->setFlat(true);
  butt->setIconSize(QSize(32, 32));
  butt->setCheckable(true);

  // Insert the button into the button group
  addButtons->addButton(butt);

  // Insert the button into the GUI
  int layoutPos = addIndexToLayoutPos(index);  // +1 for spacer in the layout
  ui->taskButtonsLayout->insertWidget(layoutPos, butt);

  // Connect the button to a slot
  connect(butt, SIGNAL(clicked()), this, SLOT(addTpButtonPushed()));

  // Check the new button
  butt->setChecked(checked);

  adjustAddTpText();
}

void TaskDeclPanel::initFromFile(TaskFile* file) {
  const TaskData* fileData = file->beginRead();

  // Iterate over task points in the task file
  int position = 0;
  const TaskPoint* tp = fileData->getTaskPoint(position);
  while (tp) {
    newTurnpointButton(position, tp->getName());
    newAddTpButton(position+1);

    position++;
    tp = fileData->getTaskPoint(position);
  }

  file->endRead();

  connect(file, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
  dataChanged();
}

void TaskDeclPanel::adjustAddTpText() {
  // Find out whether the text should be visible or not
  bool visible = true;
  if (ui->taskButtonsLayout->count() > 3) {  // 3 = 2*spacer + first button
    visible = false;
  }

  // Adjust text for all add turnpoint buttons in the GUI
  for (int i = 1; i < ui->taskButtonsLayout->count(); i+=2) {
    QLayoutItem* item = ui->taskButtonsLayout->itemAt(i);
    QWidget* widget = item->widget();
    QPushButton* addButton = qobject_cast<QPushButton*>(widget);
    if (!addButton) {
      qDebug() << "addButton is not an addButton?? Whaaat!?";
      return;
    }

    if (visible) {
      addButton->setText(addTpText);
    } else {
      addButton->setText("");
    }
  }
}

void TaskDeclPanel::uncheckAllAddTpButtons() {
  foreach(QAbstractButton* toUntoggle, addButtons->buttons()) {
    toUntoggle->setChecked(false);
  }
}

bool TaskDeclPanel::tpButtonExists(int pos) {
  int layoutPos = tpIndexToLayoutPos(pos);
  QLayoutItem* item = ui->taskButtonsLayout->itemAt(layoutPos);
  if (!item) return false;
  QWidget* widget = item->widget();
  if (!widget) return false;
  TaskPointButton* button = qobject_cast<TaskPointButton*>(widget);
  if (!button) return false;
  return true;
}

bool TaskDeclPanel::tpButtonCorrect(int pos, const TaskPoint* point) {
  int layoutPos = tpIndexToLayoutPos(pos);
  QLayoutItem* item = ui->taskButtonsLayout->itemAt(layoutPos);
  if (!item) return false;
  QWidget* widget = item->widget();
  if (!widget) return false;
  TaskPointButton* button = qobject_cast<TaskPointButton*>(widget);
  if (!button) {
    qDebug() << "unable to cast in tpButtonCorrect method";
    return false;
  }

  if (button->getName() == point->getName()) return true;
  else
    return false;
}

void TaskDeclPanel::updateTpButton(int pos, const TaskPoint* point) {
  int layoutPos = tpIndexToLayoutPos(pos);
  QLayoutItem* item = ui->taskButtonsLayout->itemAt(layoutPos);
  QWidget* widget = item->widget();
  TaskPointButton* button = qobject_cast<TaskPointButton*>(widget);
  if (!button) {
    qDebug() << "unable to cast in updateTpButton method";
    return;
  }

  button->setName(point->getName());
}

void TaskDeclPanel::updateAddTpButton(int pos, bool checkState) {
  int layoutPos = addIndexToLayoutPos(pos);
  QLayoutItem* item = ui->taskButtonsLayout->itemAt(layoutPos);
  QWidget* widget = item->widget();
  QAbstractButton* button = qobject_cast<QAbstractButton*>(widget);
  if (!button) {
    qDebug() << "unable to cast in updateAddTpButton method";
    return;
  }

  button->setChecked(checkState);
}

void TaskDeclPanel::removeTpButtons(int pos) {
  int layoutPos = tpIndexToLayoutPos(pos);
  // The ownership of the QLayoutItems returns to us -> we have to delete them
  QLayoutItem* item1 =  ui->taskButtonsLayout->takeAt(layoutPos);  // Top frame
  // Add button after it
  QLayoutItem* item2 =  ui->taskButtonsLayout->takeAt(layoutPos);
  delete item1->widget();
  delete item2->widget();
  delete item1;
  delete item2;

  // Check whether the add button is not single again
  adjustAddTpText();
}

int TaskDeclPanel::tpIndexToLayoutPos(int index) const {
  return 2*index + 2;  // +2 for spacer and the first add button
}

int TaskDeclPanel::tpLayoutPosToIndex(int pos) const {
  return (pos-2) / 2;
}

int TaskDeclPanel::addIndexToLayoutPos(int index) const {
  return 2*index + 1;  // +1 for the spacer
}

int TaskDeclPanel::addLayoutPosToIndex(int pos) const {
  return (pos-1) / 2;
}

}  // End namespace Updraft

