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
  int buttIndex = (layoutIdx - 1) / 2;
  return buttIndex;
}

void TaskDeclPanel::addTpButtonPushed() {
  if (!sender()) {  // Sender might be NULL in some threaded scenarions...
    qDebug() << "sender() was NULL in a slot. See Qt documentation for info";
    return;
  }

  /*
  // Find the index of the pushed button
  QPushButton* butt = qobject_cast<QPushButton*>(sender());
  int layoutIndex = ui->taskButtonsLayout->indexOf(butt);
  int buttIndex = (layoutIndex - 1) / 2;

  // Create new buttons
  newTurnpointButton(buttIndex);
  newAddTpButton(buttIndex + 1);

  // Hide/unhide the add button text
  adjustAddTpText();
  */
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
  int idx = ui->taskButtonsLayout->indexOf(topFrame);
  // The ownership of the QLayoutItems returns to us -> we have to delete them
  QLayoutItem* item1 =  ui->taskButtonsLayout->takeAt(idx);  // Top frame
  // Add button after it
  QLayoutItem* item2 =  ui->taskButtonsLayout->takeAt(idx);
  delete item1->widget();
  delete item2->widget();
  delete item1;
  delete item2;

  // Check whether the add button is not single again
  adjustAddTpText();
}

void TaskDeclPanel::saveButtonPushed() {
  taskLayer->save();
}

void TaskDeclPanel::newTurnpointButton(int index, const QString& name) {
  // Check the index argument
  if (index > ui->taskButtonsLayout->count() - 2) {  // 2 for the spacers
    qDebug() << "Wrong index parameter in TaskDeclPanel::newTurnpointButton: "
      << index;
    return;
  }

  // Create the GUI:
  // +-------- topFrame -------------------+
  // | number     name     +- closeFrame -+|
  // |                     |  quitButton  ||
  // |  "1."  "TextLabel"  |      [X]     ||
  // |                     +--------------+|
  // +-------------------------------------+
  QFrame* topFrame = new QFrame();
  QHBoxLayout* topFrameLayout  = new QHBoxLayout();
  topFrame->setLayout(topFrameLayout);

  // number
  QLabel* number = new QLabel(QString("%1. ").arg(index+1));
  topFrameLayout->addWidget(number);

  // name
  QLabel* nameWidget = new QLabel(name);
  topFrameLayout->addWidget(nameWidget);

  // closeFrame
  QFrame* closeFrame = new QFrame();
  QVBoxLayout* closeFrameLayout = new QVBoxLayout();
  closeFrame->setLayout(closeFrameLayout);

  QPushButton* quitButton =
    new QPushButton(QIcon(":/taskdeclpanel/icons/delete_tp_icon.png"), "");
  quitButton->setFlat(true);
  quitButton->setIconSize(QSize(8, 8));
  // Connect the quit button to a slot
  connect(quitButton, SIGNAL(clicked()), this, SLOT(removeTpButtonPushed()));

  closeFrameLayout->addWidget(quitButton);
  closeFrameLayout->addStretch();

  topFrameLayout->addWidget(closeFrame);

  // Insert the GUI into the panel GUI
  int layoutIndex = 2*index + 2;  // +2 for spacer and first add task button
  ui->taskButtonsLayout->insertWidget(layoutIndex, topFrame);
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
  int layoutIndex = 2*index + 1;  // +1 for spacer in the layout
  ui->taskButtonsLayout->insertWidget(layoutIndex, butt);

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

}  // End namespace Updraft

