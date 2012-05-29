#include "turnpoints.h"
#include "tpfilecupadapter.h"

namespace Updraft {

// Definition of global pointer to coreinterface.
CoreInterface *g_core = NULL;

TurnPoints::TurnPoints()
  : mapLayerGroup(NULL) {
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

void TurnPoints::initialize(CoreInterface *coreInterface) {
  g_core = coreInterface;

  g_core->addSettingsGroup(
    "Turnpoints", tr("Turn-points Options"));
  settings.push_back(g_core->addSetting("Turnpoints:labelColourR",
    "Colour of the turnpoint labels - RED", 1.0, true));
  settings.push_back(g_core->addSetting("Turnpoints:labelColourG",
    "Colour of the turnpoint labels - GREEN", 1.0, true));
  settings.push_back(g_core->addSetting("Turnpoints:labelColourB",
    "Colour of the turnpoint labels - BLUE", 1.0, true));
  settings.push_back(g_core->addSetting("Turnpoints:labelColourA",
    "Colour of the turnpoint labels - ALPHA", 1.0, true));
  settings.push_back(g_core->addSetting("Turnpoints:labelMaxScale",
    "Maximum scale for label", 100.0, true));
  settings.push_back(g_core->addSetting("Turnpoints:labelMinScale",
    "Minimum scale for label", 10.0, true));
  settings.push_back(g_core->addSetting("Turnpoints:labelDrawDist",
    "Minimum distance from camera for label to draw.", 4000.0, true));
  settings.push_back(g_core->addSetting("Turnpoints:labelSize",
    "Labels font size", 20.0, true));

  g_core->addSetting("Turnpoints:labelColor",
    tr("Color of the turnpoints labels"),
    QColor(Qt::white));

  mapLayerGroup = g_core->createMapLayerGroup(tr("Turn-points"));

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

void TurnPoints::fillContextMenu(MapObject* obj, MenuInterface* menu) {
  QAction* action = new QAction("Dummy action", NULL);
  menu->appendAction(action, true);
}

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
  sender->setVisible(itLayer.value()->isDisplayed());
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
    mapLayerGroup->insertMapLayer(turnPointsLayer->getNode(),
      file->getFileName(), -1);

  layers.insert(mapLayer, turnPointsLayer);

  mapLayer->connectSignalChecked(this,
    SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
}

Q_EXPORT_PLUGIN2(turnpoints, TurnPoints)

}  // End namespace Updraft
