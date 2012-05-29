#include "maplayer.h"

namespace Updraft {
namespace Core {

void MapLayer::connectSignalDisplayed(const QObject *receiver,
  const char *method) {
  connect(this, SIGNAL(displayed(bool, MapLayerInterface*)), receiver, method);
}

void MapLayer::connectSignalChecked(const QObject *receiver,
  const char *method) {
  connect(this, SIGNAL(checked(bool, MapLayerInterface*)), receiver, method);
}

void MapLayer::connectSlotSetVisibility(const QObject *sender,
  const char* method) {
  connect(sender, method, this, SLOT(setVisibility(bool)));
}

void MapLayer::connectCheckedToVisibility() {
  connect(this, SIGNAL(checked(bool, MapLayerInterface*)),
    this, SLOT(setVisibility(bool)));
}

void MapLayer::emitDisplayed(bool value) {
  emit displayed(value, this);
}

void MapLayer::emitChecked(bool value) {
  emit checked(value, this);
}

void MapLayer::setVisibility(bool value) {
  this->setVisible(value);
}

}  // End namespace Core
}  // End namespace Updraft
