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

  automaticColors.append(QPair<QColor, int>(Qt::red, 0));
  automaticColors.append(QPair<QColor, int>(Qt::green, 0));
  automaticColors.append(QPair<QColor, int>(Qt::blue, 0));
  automaticColors.append(QPair<QColor, int>(Qt::cyan, 0));
  automaticColors.append(QPair<QColor, int>(Qt::magenta, 0));
  automaticColors.append(QPair<QColor, int>(Qt::yellow, 0));
  automaticColors.append(QPair<QColor, int>(Qt::gray, 0));
}

void IgcViewer::deinitialize() {
  foreach(OpenedFile* f, opened) {
    delete f;
  }

  qDebug("igcviewer unloaded");
}

bool IgcViewer::fileOpen(const QString &filename, int roleId) {
  OpenedFile* f = new OpenedFile();

  QColor c = findAutomaticColor();
  if (!f->init(this, filename, c)) {
    delete f;
    freeAutomaticColor(c);
    return false;
  }

  foreach(OpenedFile* other, opened) {
    f->updateScales(other);
    other->updateScales(f);

    other->redraw();
  }

  opened.append(f);

  return true;
}

void IgcViewer::fileClose(OpenedFile *f) {
  opened.removeAll(f);

  freeAutomaticColor(f->getAutomaticColor());

  foreach(OpenedFile *other, opened) {
    other->resetScales();
  }

  if (opened.count() == 0) {
    return;
  }

  OpenedFile *first = opened[0];

  for (int i = 1; i < opened.count(); ++i) {
    first->updateScales(opened[i]);
  }

  first->redraw();

  for (int i = 1; i < opened.count(); ++i) {
    opened[i]->updateScales(first);
    opened[i]->redraw();
  }
}

QColor IgcViewer::findAutomaticColor() {
  int min = 0;
  for (int i = 1; i < automaticColors.count(); ++i) {
    if (automaticColors[i].second < automaticColors[min].second) {
      min = i;
    }
  }
  ++automaticColors[min].second;

  return automaticColors[min].first;
}

void IgcViewer::freeAutomaticColor(QColor c) {
  for (int i = 1; i < automaticColors.count(); ++i) {
    if (automaticColors[i].first == c) {
      --automaticColors[i].second;

      return;
    }
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

