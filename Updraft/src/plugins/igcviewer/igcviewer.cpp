#include "igcviewer.h"

#include <assert.h>

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
  foreach(OpenedFile* f, opened) {
    delete f;
  }

  qDebug("igcviewer unloaded");
}

bool IgcViewer::fileOpen(const QString &filename, int roleId) {
  OpenedFile* f = new OpenedFile();

  if (!f->init(this, filename)) {
    delete f;
    return false;
  }

  foreach(OpenedFile* other, opened) {
    Q_ASSERT(other->igcInfoList.count() == f->igcInfoList.count());

    for (int i = 0; i < f->igcInfoList.count(); ++i) {
      IgcInfo *info1 = f->igcInfoList[i];
      IgcInfo *info2 = other->igcInfoList[i];

      info1->addGlobalScale(info2->globalMin(), info2->globalMax());
    }
  }

  foreach(OpenedFile* other, opened) {
    Q_ASSERT(other->igcInfoList.count() == f->igcInfoList.count());

    for (int i = 0; i < f->igcInfoList.count(); ++i) {
      IgcInfo *info1 = f->igcInfoList[i];
      IgcInfo *info2 = other->igcInfoList[i];

      info2->addGlobalScale(info1->globalMin(), info1->globalMax());
    }
  }

  opened.append(f);

  redrawAll();

  return true;
}

void IgcViewer::fileClose(OpenedFile *f) {
  opened.removeAll(f);

  foreach(OpenedFile *other, opened) {
    for (int i = 0; i < f->igcInfoList.count(); ++i) {
      other->igcInfoList[i]->resetGlobalScale();
    }
  }

  if (opened.count() <= 1) {
    // There is no point in global scale if there is only one file.
    return;
  }

  for (int i = 0; i < opened[0]->igcInfoList.count(); ++i) {
    qreal min = opened[0]->igcInfoList[i]->min();
    qreal max = opened[0]->igcInfoList[i]->max();

    foreach(OpenedFile* other, opened) {
      min = qMin(other->igcInfoList[i]->min(), min);
      max = qMax(other->igcInfoList[i]->max(), max);
    }

    foreach(OpenedFile* other, opened) {
      other->igcInfoList[i]->addGlobalScale(min, max);
    }
  }

  redrawAll();
}

void IgcViewer::redrawAll() {
  foreach(OpenedFile *f, opened) {
    f->redraw();
  }
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

