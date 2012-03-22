#include "turnpoints.h"
#include "tpfilecupadapter.h"

namespace Updraft {

TurnPoints::TurnPoints()
  : nextLayerId(1), mapLayerGroup(NULL) {
  cupTPsReg.category = CATEGORY_PERSISTENT;
  cupTPsReg.extension = ".cup";
  cupTPsReg.typeDescription = tr("SeeYou turn-points file");
  cupTPsReg.roleDescription = tr("Import turn-points");
  cupTPsReg.importDirectory = "turnpoints";
  cupTPsReg.roleId = IMPORT_CUP_FILE;
  cupTPsReg.plugin = this;
}

TurnPoints::~TurnPoints() {
  unloadFiles();
}

QString TurnPoints::getName() {
  return QString("turnpoints");
}

unsigned TurnPoints::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void TurnPoints::initialize() {
  qDebug("turnpoints loaded");

  mapLayerGroup = core->createMapLayerGroup(tr("Turn-points"));

  core->registerFiletype(cupTPsReg);

  loadImportedFiles();
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

bool TurnPoints::wantsToHandleClick(MapObject* obj) {
  if (qobject_cast<TPMapObject*>(obj)) return true;
  return false;
}

void TurnPoints::handleMouseEvent(MapObject* obj, const EventInfo* evt) {
  qDebug(QString("Clicked a map object named %1").arg(obj->name).toAscii().data());
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
  sender->setVisible(itLayer.value()->isDisplayed());
}

void TurnPoints::loadImportedFiles() {
  QDir dir(core->getDataDirectory() + "/" + cupTPsReg.importDirectory);

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
  nextLayerId = 1;
}

void TurnPoints::addLayer(TPFile *file) {
  if (mapLayerGroup == NULL || file == NULL) {
    return;
  }

  // Create new layer item, build scene.
  TPLayer *turnPointsLayer = new TPLayer(true,
    mapLayerGroup->getObjectPlacer(), file,
    core->getDataDirectory(), this);

  // Create new mapLayer in mapLayerGroup, assign osgNode and file name.
  Updraft::MapLayerInterface* mapLayer =
    mapLayerGroup->insertMapLayer(turnPointsLayer->getNode(),
      file->getFileName(), -1);

  layers.insert(mapLayer, turnPointsLayer);

  mapLayer->connectSignalDisplayed(this,
    SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
}

Q_EXPORT_PLUGIN2(turnpoints, TurnPoints)

}  // End namespace Updraft
