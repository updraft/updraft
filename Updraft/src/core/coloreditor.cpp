#include "coloreditor.h"

#include <QColorDialog>

namespace Updraft {
namespace Core {

ColorEditor::ColorEditor(QWidget *parent)
  : QPushButton(parent) {
  connect(this, SIGNAL(clicked()), this, SLOT(onClick()));
}

const QColor &ColorEditor::color() {
  return c;
}

void ColorEditor::setColor(const QColor &c) {
  this->c = c;
  setText(QString("R/G/B/A: %1/%2/%3/%4")
    .arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha()));
}

void ColorEditor::onClick() {
  setColor(QColorDialog::getColor(c, this));
}

}  // End namespace Core
}  // End namespace Updraft

