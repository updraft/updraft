#include "ellipsoidnameeditor.h"

#include "updraft.h"
#include "util/ellipsoid.h"

namespace Updraft {
namespace Core {

EllipsoidNameEditor::EllipsoidNameEditor(QWidget *parent)
  : QComboBox(parent) {
  foreach(Util::Ellipsoid* ellipsoid, updraft->ellipsoids) {
    addItem(
      ellipsoid->getName(),
      Util::Ellipsoid::getEllipsoidTypeName(ellipsoid->getType()));
  }
}

EllipsoidName EllipsoidNameEditor::name() {
  return EllipsoidName(itemData(currentIndex()).toString());
}

void EllipsoidNameEditor::setName(const EllipsoidName &n) {
  for (int i = 0; i < count(); ++i) {
    if (itemData(i).toString() == n.asQString()) {
      setCurrentIndex(i);
      return;
    }
  }

  addItem(n.asQString(), n.asQString());
  setCurrentIndex(count() - 1);
}

}  // End namespace Core
}  // End namespace Updraft

