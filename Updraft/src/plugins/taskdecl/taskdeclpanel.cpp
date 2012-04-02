#include <QDebug>
#include <QPushButton>
#include <QSize>
#include <QLayoutItem>

#include "taskdeclpanel.h"
#include "ui_taskdeclpanel.h"

TaskDeclPanel::TaskDeclPanel(QWidget *parent, Qt::WFlags flags)
  : QWidget(parent, flags),
  addTpText("Add turnpoint"),
  ui(new Ui::TaskDeclPanel) {
  // Create the UI
  ui->setupUi(this);

  newAddTpButton(0);
}

TaskDeclPanel::~TaskDeclPanel() {
}

void TaskDeclPanel::addTpButtonPushed() {
  if (!sender()) {  // Sender might be NULL in some threaded scenarions...
    qDebug() << "sender() was NULL in a slot. See Qt documentation for info";
    return;
  }

  // Find the index of the pushed button
  QPushButton* butt = qobject_cast<QPushButton*>(sender());
  int layoutIndex = ui->taskButtonsLayout->indexOf(butt);
  int buttIndex = (layoutIndex - 1) / 2;

  // Create new buttons
  newTurnpointButton(buttIndex);
  newAddTpButton(buttIndex + 1);

  // Hide/unhide the add button text
  adjustAddTpText();
}

void TaskDeclPanel::newTurnpointButton(int index) {
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
  QLabel* number = new QLabel("1. ");  // TODO(cestmir): use real data
  topFrameLayout->addWidget(number);

  // name
  QLabel* name   = new QLabel("TextLabel");
  topFrameLayout->addWidget(name);

  // closeFrame
  QFrame* closeFrame = new QFrame();
  QVBoxLayout* closeFrameLayout = new QVBoxLayout();
  closeFrame->setLayout(closeFrameLayout);

  QPushButton* quitButton =
    new QPushButton(QIcon(":/taskdeclpanel/icons/delete_tp_icon.png"), "");
  quitButton->setFlat(true);
  quitButton->setIconSize(QSize(8, 8));

  closeFrameLayout->addWidget(quitButton);
  closeFrameLayout->addStretch();

  topFrameLayout->addWidget(closeFrame);

  // Insert the GUI into the panel GUI
  int layoutIndex = 2*index + 2;  // +2 for spacer and first add task button
  ui->taskButtonsLayout->insertWidget(layoutIndex, topFrame);
}

void TaskDeclPanel::newAddTpButton(int index) {
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

  // Insert the button into the GUI
  int layoutIndex = 2*index + 1;  // +1 for spacer in the layout
  ui->taskButtonsLayout->insertWidget(layoutIndex, butt);

  // Connect the button to a slot
  connect(butt, SIGNAL(clicked()), this, SLOT(addTpButtonPushed()));
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

