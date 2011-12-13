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
}

void TaskDeclaration::deinitialize() {
  qDebug("taskdecl unloaded");
}

Q_EXPORT_PLUGIN2(taskdecl, TaskDeclaration)

}  // End namespace Core
}  // End namespace Updraft

