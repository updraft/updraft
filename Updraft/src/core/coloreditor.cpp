#include "coloreditor.h"

#include <QColorDialog>
#include <QPushButton>
#include <QVBoxLayout>

namespace Updraft {
namespace Core {

ColorEditor::ColorEditor(QWidget *parent)
  : QWidget(parent) {
  button = new QPushButton();

  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(button);
  this->setLayout(layout);

  connect(button, SIGNAL(clicked()), this, SLOT(onClick()));
}

const QColor &ColorEditor::color() {
  return c;
}

void ColorEditor::setColor(const QColor &c) {
  this->c = c;
  button->setText(QString("R/G/B/A: %1/%2/%3/%4")
    .arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha()));

  // Change colors of the button to make it nicer
  QColor textColor;
  if (c.lightness() > 164) {
    textColor = "#000000";
  } else {
    textColor = "#FFFFFF";
  }
  QString style = QString("background-color: ") + c.name() +
    "; color: " + textColor.name() + ";";
  button->setStyleSheet(style);
}

void ColorEditor::onClick() {
  QColor color = QColorDialog::getColor(c, this, QString(),
    QColorDialog::ShowAlphaChannel);

  if (!color.isValid()) {
    return;
  }

  setColor(color);
}

}  // End namespace Core
}  // End namespace Updraft

