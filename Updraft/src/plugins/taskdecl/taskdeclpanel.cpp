#include "taskdeclpanel.h"
#include "ui_taskdeclpanel.h"

TaskDeclPanel::TaskDeclPanel(QWidget *parent, Qt::WFlags flags)
  : QWidget(parent, flags), ui(new Ui::TaskDeclPanel) {
  ui->setupUi(this);
}

TaskDeclPanel::~TaskDeclPanel() {
}
