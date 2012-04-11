#include "taskpointbutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>

namespace Updraft {

TaskPointButton::TaskPointButton(int pos, const QString& name):
  QFrame(NULL) {
  topFrameLayout = new QHBoxLayout();
  this->setLayout(topFrameLayout);

  // number
  number = new QLabel(QString("%1. ").arg(pos+1));
  topFrameLayout->addWidget(number);

  // name
  nameWidget = new QLabel(name);
  topFrameLayout->addWidget(nameWidget);

  // closeFrame
  closeFrame = new QFrame();
  closeFrameLayout = new QVBoxLayout();
  closeFrame->setLayout(closeFrameLayout);

  quitButton =
    new QPushButton(QIcon(":/taskdeclpanel/icons/delete_tp_icon.png"), "");
  quitButton->setFlat(true);
  quitButton->setIconSize(QSize(8, 8));

  // TODO(cestmir): react to close button

  closeFrameLayout->addWidget(quitButton);
  closeFrameLayout->addStretch();

  topFrameLayout->addWidget(closeFrame);

  // Save the position
  taskPointPos = pos;
}

void TaskPointButton::setName(const QString& newName) {
  nameWidget->setText(newName);
}

QString TaskPointButton::getName() {
  return nameWidget->text();
}

}  // End namespace Updraft

