#include "airspaces.h"
#include "../../libraries/openairspace/openairspace.h"


namespace Updraft {
namespace Core {

Airspaces::Airspaces() {
  qDebug("Loading deafault for testing");
  OpenAirspace::Parser P("c:/Updraft/CZ2011.txt");
  size_t as = P.size();
  QString aaa = P.GetName(as-1);

  OAirspaceFileReg.category = CATEGORY_PERSISTENT;
  OAirspaceFileReg.extension = ".txt";
  OAirspaceFileReg.typeDescription = tr("Open Airspace file");
  OAirspaceFileReg.roleDescription = tr("Import Airspace");
  OAirspaceFileReg.importDirectory = "airspaces";
  OAirspaceFileReg.roleId = 0;
  OAirspaceFileReg.plugin = this;
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

