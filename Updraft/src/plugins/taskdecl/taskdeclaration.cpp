#include "taskdeclaration.h"
#include <osgEarthUtil/ElevationManager>
#include "taskdeclpanel.h"
#include "../turnpoints/tpmapobject.h"
#include "core/mapmapobject.h"
#include "eventinfo.h"

namespace Updraft {

// Definition of global pointer to coreinterface.
CoreInterface *g_core = NULL;

TaskDeclaration::TaskDeclaration()
  : mapLayerGroup(NULL) {
}

TaskDeclaration::~TaskDeclaration() {
  unloadFiles();
}

QString TaskDeclaration::getName() {
  return QString("taskdecl");
}

void TaskDeclaration::initialize(CoreInterface *coreInterface) {
  g_core = coreInterface;

  mapLayerGroup = g_core->createMapLayerGroup(tr("Tasks"));
  mapLayerGroup->setId("tasks");
  mapLayerGroup->connectCheckedToVisibility();

  // Menu - separator
  QAction* sepAction = new QAction(this);
  sepAction->setSeparator(true);
  g_core->getSystemMenu(MENU_FILE)->insertAction(1, sepAction);

  // Menu - New Task

  QAction* createTaskAction = new QAction(tr("New Task"), this);
  connect(createTaskAction, SIGNAL(triggered()), this, SLOT(createTask()));

  g_core->getSystemMenu(MENU_FILE)->insertAction(2, createTaskAction);

  // Menu - Save

  QAction* saveTaskAction = new QAction(tr("Save Task"), this);
  saveTaskAction->setShortcuts(QKeySequence::Save);
  connect(saveTaskAction, SIGNAL(triggered()), this, SLOT(saveTask()));

  g_core->getSystemMenu(MENU_FILE)->insertAction(3, saveTaskAction);

  // Menu - Save As

  QAction* saveAsTaskAction = new QAction(tr("Save Task As..."), this);
  saveAsTaskAction->setShortcuts(QKeySequence::SaveAs);
  connect(saveAsTaskAction, SIGNAL(triggered()), this, SLOT(saveTaskAs()));

  g_core->getSystemMenu(MENU_FILE)->insertAction(4, saveAsTaskAction);

  // File type registration

  FileRegistration fileReg;
  fileReg.category = CATEGORY_TEMPORARY;
  fileReg.extension = ".tsk";
  fileReg.typeDescription = tr("Task file");
  fileReg.roleDescription = tr("Open task");
  fileReg.roleId = OPEN_TASK_FILE;
  fileReg.plugin = this;

  g_core->registerFiletype(fileReg);

  qDebug("taskdecl loaded");
}

void TaskDeclaration::deinitialize() {
  unloadFiles();
  qDebug("taskdecl unloaded");
}

bool TaskDeclaration::wantsToHandleClick(MapObject* obj) {
  if (obj->getObjectTypeName() != TPMapObject::getClassName() &&
      obj->getObjectTypeName() != Core::MapMapObject::getClassName())
    return false;

  TaskLayer* layer = getActiveLayer();
  if (!layer) return false;

  return layer->getTaskDeclPanel()->hasToggledButton();
}

void TaskDeclaration::handleClick(MapObject* obj, const EventInfo* evt) {
  TaskLayer* layer = getActiveLayer();
  if (!layer) return;

  if (obj->getObjectTypeName() == TPMapObject::getClassName()) {
    layer->newTaskPoint(static_cast<TPMapObject*>(obj)->turnPoint);
  } else if (obj->getObjectTypeName() == Core::MapMapObject::getClassName()) {
    const osg::EllipsoidModel* ellipsoidModel =
      g_core->getCurrentMapEllipsoid();
    Util::Location mapLoc;
    qreal latRad, lonRad, alt;
    ellipsoidModel->convertXYZToLatLongHeight(
      evt->intersection.x(),
      evt->intersection.y(),
      evt->intersection.z(),
      latRad,
      lonRad,
      alt);
    mapLoc.latFromRadians(latRad);
    mapLoc.lonFromRadians(lonRad);
    mapLoc.alt = alt;
    double elevation, resolution;
    g_core->getElevationManager()->getElevation(
      mapLoc.lon, mapLoc.lat, 0, NULL, elevation, resolution);
    mapLoc.alt += elevation + 100;
    layer->newTaskPoint(mapLoc);
  }
  return;
}

bool TaskDeclaration::fileOpen(const QString &filename, int roleId) {
  TaskFile *file = NULL;

  switch (roleId) {
    case OPEN_TASK_FILE:
      file = new TaskFile(filename);
      break;
  }

  if (file != NULL) {
    addLayer(file);
    return true;
  }

  return false;
}

void TaskDeclaration::fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename) {
  // TODO(Tom): Implement
}

void TaskDeclaration::createTask() {
  addLayer(new TaskFile());
}

void TaskDeclaration::saveTask() {
  TaskLayer *activeLayer = getActiveLayer();
  if (activeLayer == NULL) {
    return;
  }

  activeLayer->save();
}

void TaskDeclaration::saveTaskAs() {
  TaskLayer *activeLayer = getActiveLayer();
  if (activeLayer == NULL) {
    return;
  }

  activeLayer->saveAs();
}

TaskLayer* TaskDeclaration::getActiveLayer() {
  foreach(TaskLayer *layer, layers) {
    if (layer->isTabSelected()) {
      return layer;
    }
  }
  return NULL;
}

void TaskDeclaration::unloadFiles() {
  foreach(TaskLayer *layer, layers) {
    delete layer;
  }
  layers.clear();
}

void TaskDeclaration::addLayer(TaskFile *file) {
  if (file == NULL) {
    return;
  }

  // Create and add new layer item, build scene.
  layers.append(new TaskLayer(true, this, file, getIndexForNewTask()));
}

int TaskDeclaration::getIndexForNewTask() {
  int maxIndex = 0;

  // Finds maximal index of unstored new tasks.
  foreach(TaskLayer *layer, layers) {
    switch (layer->getStorageState()) {
      case TaskFile::UNSTORED_EMPTY:
      case TaskFile::UNSTORED_EDITED:
        maxIndex = qMax(layer->getNewTaskIndex(), maxIndex);
        break;
      default:
        // do nothing
        break;
    }
  }

  return maxIndex + 1;
}

Q_EXPORT_PLUGIN2(taskdecl, TaskDeclaration)

}  // End namespace Updraft
