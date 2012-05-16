#include "directoryeditor.h"

#include <QFileDialog>

namespace Updraft {
namespace Core {

DirectoryEditor::DirectoryEditor(QWidget *parent): QPushButton(parent) {
  connect(this, SIGNAL(clicked()), this, SLOT(onClick()));
}

QDir DirectoryEditor::directory() {
  QDir retVal(this->text());
  return retVal;
}

void DirectoryEditor::setDirectory(const QDir &dir) {
  this->setText(dir.absolutePath());
}

void DirectoryEditor::onClick() {
  QDir dir = directory();
  QString d = QFileDialog::getExistingDirectory(
    this,
    "Select a directory",
    dir.absolutePath());
  if (d != "") {
    this->setText(dir.absolutePath());
  }
}

}  // End namespace Core
}  // End namespace Updraft

