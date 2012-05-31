#include "airspaces.h"


namespace Updraft {
namespace Airspaces {

// Definition of global pointer to coreinterface.
CoreInterface *g_core = NULL;

Airspaces::Airspaces() {
  mapLayers = NULL;
  mapLayerGroup = NULL;
}

QString Airspaces::getName() {
  return QString("airspaces");
}

void Airspaces::initialize(CoreInterface *coreInterface) {
  g_core = coreInterface;

  // File type registration
  OAirspaceFileReg.category = CATEGORY_PERSISTENT;
  OAirspaceFileReg.extension = ".txt";
  OAirspaceFileReg.typeDescription = tr("Open Airspace file");
  OAirspaceFileReg.roleDescription = tr("Import Open Airspace");
  OAirspaceFileReg.importDirectory = "airspaces";
  OAirspaceFileReg.roleId = IMPORT_OPENAIRSPACE_FILE;
  OAirspaceFileReg.plugin = this;
  g_core->registerFiletype(OAirspaceFileReg);

  // Create map layers items in the left pane.
  mapLayerGroup = g_core->createMapLayerGroup(tr("Airspace"));
  // engine = new oaEngine(mapLayerGroup);
  mapLayers = new QVector<MapLayerInterface*>();

  loadImportedFiles();

  qDebug("airspaces loaded");
}

void Airspaces::mapLayerDisplayed(bool value, MapLayerInterface* sender) {
  sender->setVisible(value);
  // reloadAirspaces();
}

void Airspaces::reloadAirspaces() {
  // delete engine;
  // engine = new oaEngine(mapLayerGroup);

  // loadImportedFiles();

  // mapLayerGroup

  for (int i = 0; i < mapLayers->size(); ++i) {
    mapLayerGroup->removeMapLayer(mapLayers->at(i));
  }

  loadImportedFiles();

  qDebug("airspaces reloaded");

  // deinitialize();
  // initialize();
}

void Airspaces::deinitialize() {
  // delete engine;
  // engine = NULL;
  if (mapLayers) {
    for (QVector<MapLayerInterface*>::iterator it =
      mapLayers->begin(); it < mapLayers->end(); ++it)
      delete (*it);
    delete mapLayers;
    mapLayers = NULL;
  }
  if (mapLayerGroup) {
    delete mapLayerGroup;
    mapLayerGroup = NULL;
  }
  qDebug("airspaces unloaded");
}

bool Airspaces::fileOpen(const QString& fileName, int role) {
  switch (role) {
    case IMPORT_OPENAIRSPACE_FILE:
      // draw openairspace file
      /* mapLayers = engine->Draw(fileName);
      if (!mapLayers) return false;
      for (int i = 0; i < mapLayers->size(); ++i) {
        MapLayerInterface* layer1 = mapLayers->at(i);
        // layer1->setVisible(false);
        layer1->connectSignalChecked
          (this, SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
        layer1->emitDisplayed(false);
      }
      delete mapLayers;*/

      oaEngine* engine = new oaEngine(mapLayerGroup);

      QString displayName = fileName.left(fileName.indexOf('.'));
      int cuntSlashes = displayName.count('/');
      displayName = displayName.section('/', cuntSlashes, cuntSlashes);

      mapNodes = engine->Draw(fileName);
      if (!mapNodes) return false;

      QVector<MapLayerInterface*>* layers =
        mapLayerGroup->insertMapLayerGroup(mapNodes, displayName);
      if (!layers) return false;
      if (!mapLayers)
        mapLayers = new QVector<MapLayerInterface*>();
      for (int i = 0; i < mapLayers->size(); ++i)
        mapLayers->append(layers->at(i));

      for (int i = 0; i < layers->size(); ++i) {
        layers->at(i)->connectSignalChecked
          (this, SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
      }

      // for (int i = 0; i < layers->size(); ++i) {
        // mapLayerGroup->removeMapLayer(layers->at(i));
      // }

      delete mapNodes;

      delete engine;
      engine = NULL;

      return true;
      break;
  }
  return false;
}

void Airspaces::loadImportedFiles() {
  QDir dir(g_core->getDataDirectory() + "/" + OAirspaceFileReg.importDirectory);

  if (!dir.exists()) {
    return;
  }

  QStringList filters("*" + OAirspaceFileReg.extension);
  QStringList entries = dir.entryList(filters, QDir::Files, QDir::Time);

  foreach(QString fileName, entries) {
    fileOpen(dir.absoluteFilePath(fileName), OAirspaceFileReg.roleId);
  }
}

Q_EXPORT_PLUGIN2(airspaces, Airspaces)

}  // End namespace Airspaces
}  // End namespace Updraft

