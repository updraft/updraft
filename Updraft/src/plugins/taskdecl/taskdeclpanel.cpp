#include <QDebug>
#include <QPushButton>
#include <QSize>
#include <QLayoutItem>
#include <QFileDialog>

#include "taskdeclpanel.h"
#include "ui_taskdeclpanel.h"
#include "tasklayer.h"
#include "taskfile.h"
#include "taskdata.h"
#include "taskpoint.h"
#include "taskpointbutton.h"

namespace Updraft {

TaskDeclPanel::TaskDeclPanel(QWidget *parent, Qt::WFlags flags)
  : QWidget(parent, flags),
  addTpText("Add turnpoint"),
  ui(new Ui::TaskDeclPanel) {
  // Create the UI
  ui->setupUi(this);
  addButtons = new QButtonGroup(this);

  connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveButtonPushed()));

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
  int pos = ui->taskButtonsLayout->indexOf(topFrame);
  removeTpButtons(tpLayoutPosToIndex(pos));
}

void TaskDeclPanel::saveButtonPushed() {
  taskLayer->save();
}

void TaskDeclPanel::updateButtons() {
  TaskFile* file = taskLayer->getTaskFile();

  if (!file) return;

  // Iterate over the task data
  const TaskData* data = file->getData();
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

  // Iterate over any remaining task buttons
  while (tpButtonExists(pos)) {
    removeTpButtons(pos);
  }
}

void TaskDeclPanel::newTurnpointButton(int index, const QString& name) {
  // Check the index argument
  if (index > ui->taskButtonsLayout->count() - 2) {  // 2 for the spacers
    qDebug() << "Wrong index parameter in TaskDeclPanel::newTurnpointButton: "
      << index;
    return;
  }

  TaskPointButton* button = new TaskPointButton(index, name);

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
  TaskData* fileData = file->beginEdit();

  // Iterate over task points in the task file
  int position = 0;
  TaskPoint* tp = fileData->getTaskPoint(position);
  while (tp) {
    newTurnpointButton(position, tp->getName());
    newAddTpButton(position+1);

    position++;
    tp = fileData->getTaskPoint(position);
  }

  file->endEdit(false);
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
  else return false;
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

void TaskDeclPanel::removeTpButtons(int pos) {
  int layoutPos = tpIndexToLayoutPos(pos);
  // The ownership of the QLayoutItems returns to us -> we have to delete them
  QLayoutItem* item1 =  ui->taskButtonsLayout->takeAt(layoutPos);  // Top frame
  // Add button after it
  QLayoutItem* item2 =  ui->taskButtonsLayout->takeAt(layoutPos);
  //delete item1->widget();
  delete item2->widget();
  delete item1;
  delete item2;

  // Check whether the add button is not single again
  adjustAddTpText();
}

int TaskDeclPanel::tpIndexToLayoutPos(int index) {
  return 2*index + 2;  // +2 for spacer and the first add button
}

int TaskDeclPanel::tpLayoutPosToIndex(int pos) {
  return (pos-2) / 2;
}

int TaskDeclPanel::addIndexToLayoutPos(int index) {
  return 2*index + 1;  // +1 for the spacer
}

int TaskDeclPanel::addLayoutPosToIndex(int pos) {
  return (pos-1) / 2;
}

}  // End namespace Updraft

