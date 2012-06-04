#include "directoryeditor.h"

// Only debug!
#include <QtGui>

#include <QFileDialog>

namespace Updraft {
namespace Core {

DirectoryEditor::DirectoryEditor(QWidget *parent): QPushButton(parent) {
  connect(this, SIGNAL(clicked()), this, SLOT(onClick()));
}

const QDir& DirectoryEditor::directory() const {
  return dirProp;
}

void DirectoryEditor::setDirectory(const QDir &dir) {
  dirProp =  dir;
  this->setText(dir.absolutePath());
}

void DirectoryEditor::onClick() {
  QDir dir = directory();
  QString d = QFileDialog::getExistingDirectory(
    this,
    tr("Select a directory"),
    dir.absolutePath(),
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  qDebug() << d;
  if (d != "") {
    this->setText(d);
    dirProp = d;
  }
}

}  // End namespace Core
}  // End namespace Updraft

