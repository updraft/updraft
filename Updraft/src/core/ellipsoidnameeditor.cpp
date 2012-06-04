#include "ellipsoidnameeditor.h"

#include "updraft.h"
#include "util/ellipsoid.h"

namespace Updraft {
namespace Core {

EllipsoidNameEditor::EllipsoidNameEditor(QWidget *parent)
  : QComboBox(parent) {
  foreach(Util::Ellipsoid* ellipsoid, updraft->ellipsoids) {
    addItem(ellipsoid->getName());
  }
}

EllipsoidName EllipsoidNameEditor::name() {
  return EllipsoidName(currentText());
}

void EllipsoidNameEditor::setName(const EllipsoidName &n) {
  for (int i = 0; i < count(); ++i) {
    if (itemText(i) == n) {
      setCurrentIndex(i);
      return;
    }
  }

  addItem(n.asQString());
  setCurrentIndex(count() - 1);
}

}  // End namespace Core
}  // End namespace Updraft

