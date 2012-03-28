#include "taskdeclaration.h"

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

  qDebug("taskdecl loaded");
}

void TaskDeclaration::deinitialize() {
  unloadFiles();
  qDebug("taskdecl unloaded");
}

bool TaskDeclaration::fileOpen(const QString &filename, int roleId) {
  TaskFile *file = NULL;

  switch (roleId) {
    case OPEN_TASK_FILE:
      // TODO(Tom): Implement file loading.
      // file = TPFileCupAdapter::load(filename);
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
