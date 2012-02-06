#include "igcviewer.h"

#include "openedfile.h"

namespace Updraft {
namespace IgcViewer {

QString IgcViewer::getName() {
  return QString("igcviewer");
}

unsigned IgcViewer::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void IgcViewer::initialize() {
  qDebug("igcviewer loaded");

  FileRegistration registration;
  registration.category = CATEGORY_TEMPORARY;
  registration.extension = ".igc";
  registration.typeDescription = tr("IGC flight recording");
  registration.roleDescription = tr("Open flight record");
  registration.roleId = 1;
  registration.plugin = this;

  core->registerFiletype(registration);

  mapLayerGroup = core->createMapLayerGroup(tr("IGC files"));
}

void IgcViewer::deinitialize() {
  qDebug("igcviewer unloaded");

  QList<OpenedFile*> copy = opened;
  opened.clear();

  foreach(OpenedFile* f, copy) {
    f->close();
  }
}

bool IgcViewer::fileOpen(const QString &filename, int roleId) {
  OpenedFile* f = new OpenedFile();

  if (!f->init(this, filename)) {
    delete f;
    return false;
  }

  opened.append(f);

  return true;
}

/*void IgcViewer::fileIdentification(QStringList *roles,
    QString *importDirectory, const QString &filename) {
  Igc::IgcFile igc;
  if (!igc.load(filename)) {
    qDebug() << "We couldn't load the igc file.";
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

  if (roles != NULL)
    roles->append(ident);
}*/

Q_EXPORT_PLUGIN2(igcviewer, IgcViewer)

}  // End namespace Core
}  // End namespace Updraft

