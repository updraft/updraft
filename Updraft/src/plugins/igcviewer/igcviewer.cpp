#include "igcviewer.h"

#include <QDebug>
#include "openedfile.h"

namespace Updraft {

// Definition of global pointer to coreinterface.
CoreInterface *g_core = NULL;

namespace IgcViewer {

IGCMapObject::IGCMapObject(QString objectName_, OpenedFile* file_)
  : QObject(NULL), MapObject(objectName_), file(file_) {
}

QObject* IGCMapObject::asQObject() {
  return this;
}

QString IgcViewer::getName() {
  return QString("igcviewer");
}

unsigned IgcViewer::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void IgcViewer::initialize(CoreInterface *coreInterface) {
  g_core = coreInterface;

  qDebug("igcviewer loaded");

  FileRegistration registration;
  registration.category = CATEGORY_TEMPORARY;
  registration.extension = ".igc";
  registration.typeDescription = tr("IGC flight recording");
  registration.roleDescription = tr("Open flight record");
  registration.roleId = 1;
  registration.plugin = this;

  g_core->registerFiletype(registration);

  mapLayerGroup = g_core->createMapLayerGroup(tr("IGC files"));

  automaticColors.append(QPair<QColor, int>(Qt::red, 0));
  automaticColors.append(QPair<QColor, int>(Qt::green, 0));
  automaticColors.append(QPair<QColor, int>(Qt::blue, 0));
  automaticColors.append(QPair<QColor, int>(Qt::cyan, 0));
  automaticColors.append(QPair<QColor, int>(Qt::magenta, 0));
  automaticColors.append(QPair<QColor, int>(Qt::yellow, 0));
  automaticColors.append(QPair<QColor, int>(Qt::gray, 0));

  currentColoring = 0;
}

void IgcViewer::deinitialize() {
  foreach(OpenedFile* f, opened) {
    delete f;
  }

  qDebug("igcviewer unloaded");
}

bool IgcViewer::fileOpen(const QString &filename, int roleId) {
  QFileInfo info(filename);
  QString absFilename = info.absoluteFilePath();

  if (opened.contains(absFilename)) {
    qDebug() << "already opened, ignoring";
    opened[absFilename]->selectTab();
    return true;
  }

  OpenedFile* f = new OpenedFile();

  QColor c = findAutomaticColor();
  if (!f->init(this, filename, c)) {
    delete f;
    freeAutomaticColor(c);
    return false;
  }

  IGCMapObject* mapObject = new IGCMapObject(filename, f);
  g_core->registerOsgNode(f->getNode(), mapObject);
  mapObjects.append(mapObject);

  foreach(OpenedFile* other, opened) {
    f->updateScales(other);
    other->updateScales(f);

    other->redraw();
  }

  opened.insert(filename, f);

  return true;
}

void IgcViewer::fileClose(OpenedFile *f) {
  opened.remove(f->fileName());

  freeAutomaticColor(f->getAutomaticColor());

  foreach(OpenedFile *other, opened) {
    other->resetScales();
  }

  if (opened.count() == 0) {
    return;
  }

  OpenedFile *first = *(opened.begin());

  foreach(OpenedFile *other, opened) {
    first->updateScales(other);
  }

  first->redraw();

  foreach(OpenedFile *other, opened) {
    other->updateScales(first);
    other->redraw();
  }
}

void IgcViewer::coloringChanged(int i) {
  if (i == currentColoring) {
    return;
  }

  currentColoring = i;

  foreach(OpenedFile *f, opened) {
    f->coloringChanged();
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

/// Tells whether this plugin wants to handle a mouse click event.
bool IgcViewer::wantsToHandleClick(MapObject* obj) {
  IGCMapObject* iObj = qobject_cast<IGCMapObject*>(obj->asQObject());
  if (iObj != NULL) {
    return true;
  } else {
    return false;
  }
}

/// Handles the left mouse click event on the IGC in the map.
void IgcViewer::handleClick(MapObject* obj, const EventInfo* evt) {
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

