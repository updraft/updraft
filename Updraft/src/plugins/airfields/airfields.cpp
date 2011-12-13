#include "airfields.h"

namespace Updraft {
namespace Core {

QString Airfields::getName() {
  return QString("airfields");
}

unsigned Airfields::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void Airfields::initialize() {
  qDebug("airfields loaded");
}

void Airfields::deinitialize() {
  qDebug("airfields unloaded");
}

Q_EXPORT_PLUGIN2(airfields, Airfields)

}  // End namespace Core
}  // End namespace Updraft

