#include "taskdeclaration.h"
#include "taskdeclpanel.h"
#include "../turnpoints/tpmapobject.h"

namespace Updraft {

TaskDeclaration::TaskDeclaration()
  : createTaskAction(NULL), mapLayerGroup(NULL) {
}

TaskDeclaration::~TaskDeclaration() {
  unloadFiles();
}

QString TaskDeclaration::getName() {
  return QString("taskdecl");
}

unsigned TaskDeclaration::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void TaskDeclaration::initialize() {
  mapLayerGroup = core->createMapLayerGroup(tr("Tasks"));

  createTaskAction = new QAction("New Task", this);
  connect(createTaskAction, SIGNAL(triggered()), this, SLOT(createTask()));

  core->getSystemMenu(MENU_FILE)->insertAction(1, createTaskAction);

  // File type registration

  FileRegistration fileReg;
  fileReg.category = CATEGORY_TEMPORARY;
  fileReg.extension = ".tsk";
  fileReg.typeDescription = tr("Task file");
  fileReg.roleDescription = tr("Open task");
  fileReg.roleId = OPEN_TASK_FILE;
  fileReg.plugin = this;

  core->registerFiletype(fileReg);

  qDebug("taskdecl loaded");
}

void TaskDeclaration::deinitialize() {
  unloadFiles();
  qDebug("taskdecl unloaded");
}

bool TaskDeclaration::wantsToHandleClick(MapObject* obj) {
  TPMapObject* mObj = qobject_cast<TPMapObject*>(obj->asQObject());
  if (mObj == NULL) return false;

  TaskLayer* layer = getActiveLayer();
  if (!layer) return false;

  return layer->getTaskDeclPanel()->hasToggledButton();
}

void TaskDeclaration::handleClick(MapObject* obj, const EventInfo* evt) {
  TaskLayer* layer = getActiveLayer();
  if (!layer) return;

  TPMapObject* mObj = qobject_cast<TPMapObject*>(obj->asQObject());
  if (mObj == NULL) return;

  layer->newTaskPoint(mObj->turnPoint);
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
