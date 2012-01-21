#include "airspaces.h"
#include "../../libraries/openairspace/openairspace.h"
#include "../../libraries/cup/cup.h"


namespace Updraft {
namespace Core {

Airspaces::Airspaces() {
  qDebug("Loading deafault for testing");
  // Cup::CupFile *cupFile;
  TTT();
  OpenAirspace::Parser p;
  p.Test();
}

QString Airspaces::getName() {
  return QString("airspaces");
}

unsigned Airspaces::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void Airspaces::initialize() {
  qDebug("airspaces laoded");
}

void Airspaces::deinitialize() {
  qDebug("airspaces unloaded");
}

Q_EXPORT_PLUGIN2(airspaces, Airspaces)

}  // End namespace Core
}  // End namespace Updraft

