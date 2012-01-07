#include "taskdeclaration.h"

namespace Updraft {
namespace Core {

QString TaskDeclaration::getName() {
  return QString("taskdecl");
}

unsigned TaskDeclaration::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void TaskDeclaration::initialize() {
  qDebug("taskdecl loaded");

  core->registerFiletype(".cup", "SeeYou turning points file",
    CATEGORY_PERSISTENT);
}

void TaskDeclaration::deinitialize() {
  qDebug("taskdecl unloaded");
}

bool TaskDeclaration::fileOpen(QString filename, QList<int> roles) {
  // TODO(Tom): actions on file loading (following code is only for testing)
  Cup::CupFile *cupFile = cupLoader.loadFile(filename);
  delete cupFile;
  return false;  // Not implemented
}

QStringList TaskDeclaration::fileIdentification(QString filename) {
  return QStringList("Turning points file");
}

Q_EXPORT_PLUGIN2(taskdecl, TaskDeclaration)

}  // End namespace Core
}  // End namespace Updraft

