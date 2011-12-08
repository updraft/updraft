#include "airspaces.h"

namespace Updraft {
namespace Core {

QString Airspaces::getName() {
  return QString("airspaces");
}

unsigned Airspaces::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void Airspaces::initialize() {
  qDebug("airspaces loaded");
}

void Airspaces::deinitialize() {
  qDebug("airspaces unloaded");
}

Q_EXPORT_PLUGIN2(airspaces, Airspaces)

}  // End namespace Core
}  // End namespace Updraft

