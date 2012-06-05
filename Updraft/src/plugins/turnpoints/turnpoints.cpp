#include "turnpoints.h"
#include "tpfilecupadapter.h"

namespace Updraft {

// Definition of global pointer to coreinterface.
CoreInterface *g_core = NULL;

TurnPoints::TurnPoints()
  : mapLayerGroup(NULL) {
}

TurnPoints::~TurnPoints() {
  unloadFiles();
}

QString TurnPoints::getName() {
  return QString("turnpoints");
}

void TurnPoints::initialize(CoreInterface *coreInterface) {
  g_core = coreInterface;

  mapLayerGroup = g_core->createMapLayerGroup(tr("Turn-points"));
  mapLayerGroup->connectCheckedToVisibility();

  cupTPsReg.category = CATEGORY_PERSISTENT;
  cupTPsReg.extension = ".cup";
  cupTPsReg.typeDescription = tr("SeeYou turn-points file");
  cupTPsReg.roleDescription = tr("Import turn-points");
  cupTPsReg.importDirectory = "turnpoints";
  cupTPsReg.roleId = IMPORT_CUP_FILE;
  cupTPsReg.plugin = this;
  g_core->registerFiletype(cupTPsReg);

  loadImportedFiles();

  qDebug("turnpoints loaded");
}

void TurnPoints::deinitialize() {
  unloadFiles();
  qDebug("turnpoints unloaded");
}

bool TurnPoints::fileOpen(const QString &filename, int roleId) {
  TPFile *file = NULL;

  switch (roleId) {
    case IMPORT_CUP_FILE:
      file = TPFileCupAdapter::load(filename);
      break;
  }

  if (file != NULL) {
    addLayer(file);
    return true;
  }

  return false;
}

void TurnPoints::fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename) {
  if (roles != NULL)
    roles->append(tr("Turn-points file"));
}

void TurnPoints::fillContextMenu(MapObject* obj, MenuInterface* menu) {}

bool TurnPoints::wantsToHandleClick(MapObject* obj) {
  return obj->getObjectTypeName() == TPMapObject::getClassName();
}

void TurnPoints::handleClick(MapObject* obj, const EventInfo* evt) {
  const char* format = "Clicked a map object named %1";
  qDebug(format, obj->name.toAscii().data());
}

void TurnPoints::mapLayerDisplayed(bool value, MapLayerInterface* sender) {
  if (sender == NULL) {
    return;
  }

  // Find the sender in opened layers.
  TTPLayerMap::const_iterator itLayer = layers.find(sender);
  if (itLayer == layers.end()) {
    return;
  }

  // First try to display/enable layer in Turnpoints plugin.
  // Then if it is displayed, show it also in the map.
  itLayer.value()->display(value);
  sender->setVisibility(itLayer.value()->isDisplayed());
}

void TurnPoints::loadImportedFiles() {
  QDir dir(g_core->getDataDirectory() + "/" + cupTPsReg.importDirectory);

  if (!dir.exists()) {
    return;
  }

  // Search for all cup files in import directory.
  QStringList filters("*" + cupTPsReg.extension);
  QStringList entries = dir.entryList(filters, QDir::Files, QDir::Time);

  // Load all listed files.
  foreach(QString fileName, entries) {
    fileOpen(dir.absoluteFilePath(fileName), cupTPsReg.roleId);
  }
}

void TurnPoints::unloadFiles() {
  foreach(TPLayer *layer, layers) {
    delete layer;
  }
  layers.clear();
}

void TurnPoints::addLayer(TPFile *file) {
  if (mapLayerGroup == NULL || file == NULL) {
    return;
  }

  // Create new layer item, build scene.
  TPLayer *turnPointsLayer = new TPLayer(true,
    mapLayerGroup->getObjectPlacer(), file,
    g_core->getDataDirectory(), this, settings);

  // Create new mapLayer in mapLayerGroup, assign osgNode and file name.
  Updraft::MapLayerInterface* mapLayer =
    mapLayerGroup->createMapLayer(turnPointsLayer->getNode(),
      file->getFileName(), -1);

  layers.insert(mapLayer, turnPointsLayer);

  mapLayer->connectSignalChecked(this,
    SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
  mapLayer->connectSignalContextMenuRequested(this,
    SLOT(contextMenuRequested(QPoint, MapLayerInterface*)));
  mapLayer->setFilePath(file->getFilePath());
  connect(mapLayer->getDeleteAction(), SIGNAL(triggered()),
    this, SLOT(deleteTpLayer()));
}

void TurnPoints::contextMenuRequested(QPoint pos, MapLayerInterface* sender) {
  layerToDelete = sender;

  QMenu menu;
  menu.addAction(sender->getDeleteAction());
  menu.exec(pos);
}

void TurnPoints::deleteTpLayer() {
  // layerToDelete might be an invalid pointer here, because
  // it is deleted by the context menu, but this doesn't matter
  // since we only need the value of the pointer, not the data it points to.
  delete layers[layerToDelete];
  layers.remove(layerToDelete);
}

Q_EXPORT_PLUGIN2(turnpoints, TurnPoints)

}  // End namespace Updraft
