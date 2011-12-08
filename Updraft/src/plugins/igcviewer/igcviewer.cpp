#include "igcviewer.h"

namespace Updraft {
namespace Core {

QString IgcViewer::getName() {
  return QString("igcviewer");
}

unsigned IgcViewer::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void IgcViewer::initialize() {
  qDebug("igcviewer loaded");
}

void IgcViewer::deinitialize() {
  qDebug("igcviewer unloaded");
}

Q_EXPORT_PLUGIN2(igcviewer, IgcViewer)

}  // End namespace Core
}  // End namespace Updraft

