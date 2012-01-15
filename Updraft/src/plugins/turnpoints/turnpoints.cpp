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

  core->registerFiletype(".cup", "SeeYou turning points file",
    CATEGORY_PERSISTENT);
}

void TurnPoints::deinitialize() {
  qDebug("turnpoints unloaded");
}

bool TurnPoints::fileOpen(QString filename, QList<int> roles) {
  // TODO(Tom): actions on file loading (following code is only for testing)
  Cup::CupFile *cupFile = cupLoader.loadFile(filename);
  delete cupFile;
  return false;  // Not implemented
}

QStringList TurnPoints::fileIdentification(QString filename) {
  return QStringList("Turning points file");
}

Q_EXPORT_PLUGIN2(turnpoints, TurnPoints)

}  // End namespace Core
}  // End namespace Updraft

