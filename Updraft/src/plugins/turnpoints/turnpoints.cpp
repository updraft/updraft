#include "turnpoints.h"

namespace Updraft {
namespace Core {

QString TurnPoints::getName() {
  return QString("turnpoints");
}

unsigned TurnPoints::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void TurnPoints::initialize() {
  qDebug("turnpoints loaded");

  FileRegistration registration;
  registration.category = CATEGORY_PERSISTENT;
  registration.extension = ".cup";
  registration.typeDescription = "SeeYou turning points file";
  registration.roleDescription = "Import turn points";
  registration.importDirectory = "turnpoints";
  registration.roleId = 1;
  registration.plugin = this;

  core->registerFiletype(registration);
}

void TurnPoints::deinitialize() {
  qDebug("turnpoints unloaded");
}

bool TurnPoints::fileOpen(const QString &filename, int roleId) {
  // TODO(Tom): actions on file loading (following code is only for testing)
  Cup::CupFile *cupFile = cupLoader.loadFile(filename);
  delete cupFile;
  return false;  // Not implemented
}

void TurnPoints::fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename) {
  if (roles != NULL)
    roles->append("Turning points file");
}

Q_EXPORT_PLUGIN2(turnpoints, TurnPoints)

}  // End namespace Core
}  // End namespace Updraft

