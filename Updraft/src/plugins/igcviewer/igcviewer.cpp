#include "igcviewer.h"

#include "../../libraries/igc/igc.h"

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

  core->registerFiletype(".igc", "IGC flight recording", CATEGORY_TEMPORARY);
}

void IgcViewer::deinitialize() {
  qDebug("igcviewer unloaded");
}

QStringList IgcViewer::fileIdentification(QString filename) {
  Igc::IgcFile igc;
  if (!igc.load(filename)) {
    qDebug() << "We couldn't load the igc file.";
    return QStringList();
  }

  QString ident = igc.gliderId().simplified();

  if (!ident.isEmpty()) {
    ident.append(", ");
  }

  ident.append(igc.pilot().simplified());

  if (!ident.isEmpty()) {
    ident.prepend("\n");
  }

  ident.prepend(igc.date().toString());

  return QStringList(ident);
}

Q_EXPORT_PLUGIN2(igcviewer, IgcViewer)

}  // End namespace Core
}  // End namespace Updraft

