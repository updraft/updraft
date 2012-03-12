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
/*  // File type registration
  OAirspaceFileReg.category = CATEGORY_PERSISTENT;
  OAirspaceFileReg.extension = ".cup";
  OAirspaceFileReg.typeDescription = tr("AirFields file");
  OAirspaceFileReg.roleDescription = tr("Import Open Airspace");
  OAirspaceFileReg.importDirectory = "airspaces";
  OAirspaceFileReg.roleId = IMPORT_OPENAIRSPACE_FILE;
  OAirspaceFileReg.plugin = this;
  core->registerFiletype(OAirspaceFileReg);

  // Create map layers items in the left pane.
  // QObject* airspacesMenu = core // ->createTreeItem("Airspaces");
  engine = new oaEngine(core->createMapLayerGroup("Airspaces"));
  // MapLayerInterface* layer1;
  // LoadFile("c:/Updraft/CZ2011CTR.txt", 0);
  // LoadFile("c:/Updraft/CZ2011TMA.txt", 0);
  loadImportedFiles();
  */
  qDebug("airfields loaded");
}

void Airfields::deinitialize() {
  qDebug("airfields unloaded");
}

Q_EXPORT_PLUGIN2(airfields, Airfields)

}  // End namespace Core
}  // End namespace Updraft

