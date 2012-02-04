#include "airspaces.h"


namespace Updraft {
namespace Airspaces {

Airspaces::Airspaces() {
  OAirspaceFileReg.category = CATEGORY_PERSISTENT;
  OAirspaceFileReg.extension = ".txt";
  OAirspaceFileReg.typeDescription = tr("Open Airspace file");
  OAirspaceFileReg.roleDescription = tr("Import Airspace");
  OAirspaceFileReg.importDirectory = "airspaces";
  OAirspaceFileReg.roleId = IMPORT_OPENAIRSPACE_FILE;
  OAirspaceFileReg.plugin = this;
}

QString Airspaces::getName() {
  return QString("airspaces");
}

unsigned Airspaces::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void Airspaces::initialize() {
  qDebug("Loading deafault for testing");

  // Create map layers items in the left pane.
  engine = new oaEngine(core->createMapLayerGroup("Airspaces"));
  // MapLayerInterface* layer1;
  LoadFile("c:/Updraft/CZ2011CTR.txt", 0);
  LoadFile("c:/Updraft/CZ2011TMA.txt", 0);

  qDebug("airspaces laoded");
}

void Airspaces::mapLayerDisplayed(bool value, MapLayerInterface* sender) {
  sender->setVisible(value);
}

void Airspaces::deinitialize() {
  qDebug("airspaces unloaded");
}

bool Airspaces::LoadFile(const QString& fileName, int role) {
  switch (role) {
    case IMPORT_OPENAIRSPACE_FILE:
      // draw openairspace file
      MapLayerInterface* layer1 = engine->Draw(fileName);
      layer1->connectSignalDisplayed
        (this, SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
      mapLayers.append(layer1);
      return true;
      break;
  }
  return false;
}


Q_EXPORT_PLUGIN2(airspaces, Airspaces)

}  // End namespace Airspaces
}  // End namespace Updraft

