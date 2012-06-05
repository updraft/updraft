#include "updraft.h"

#include <QDesktopServices>

#include "../maplayerinterface.h"
#include "../settinginterface.h"
#include "ui/maplayergroup.h"
#include "variantfunctions.h"
#include "ui/mainwindow.h"
#include "filetypemanager.h"
#include "pluginmanager.h"
#include "scenemanager.h"
#include "settingsmanager.h"
#include "translationmanager.h"
#include "util/ellipsoid.h"
#include "ellipsoidname.h"

namespace Updraft {
namespace Core {

Updraft::Updraft(int argc, char** argv)
  : QApplication(argc, argv) {
  splash.show();

  // Needed so that we can use custom types in QVariant
  registerMetaTypes();

  settingsManager = new SettingsManager();
  translationManager = new TranslationManager();

  createEllipsoids();
  coreSettings();

  mainWindow = new MainWindow(NULL);
  fileTypeManager = new FileTypeManager();
  sceneManager = new SceneManager();

  mainWindow->setMapWidget(sceneManager->getWidget());

  pluginManager = new PluginManager();

  QStringList args = arguments();
  for (int i = 1; i < args.count(); ++i) {
    fileTypeManager->openFile(args[i]);
  }

  dataDirectoryChangeInProgress = false;
}

Updraft::~Updraft() {
  delete sceneManager;
  delete pluginManager;
  delete fileTypeManager;
  delete settingsManager;
  delete mainWindow;

  foreach(Util::Ellipsoid *e, ellipsoids) {
    delete e;
  }
}

QString Updraft::getDataDirectory() {
  QDir dataDir = currentDataDirectory;
  dataDir.cd("data");
  return dataDir.absolutePath();
}

QDir Updraft::getTranslationDirectory() {
  return QDir(applicationDirPath());
}

Util::Ellipsoid* Updraft::getUsedEllipsoid() {
  foreach(Util::Ellipsoid* ellipsoid, ellipsoids) {
    if (ellipsoid->getName() ==
        usedEllipsoid->get().value<EllipsoidName>().asQString()) {
      return ellipsoid;
    }
  }

  return ellipsoids.first();
}

void Updraft::dataDirectoryChanged() {
  // Prevent reentry into this function
  if (dataDirectoryChangeInProgress) return;
  dataDirectoryChangeInProgress = true;

  QDir dataDir = currentDataDirectory;
  QDir newDataDir = dataDirectory->get().value<QDir>();

  if (dataDir.rename(
    dataDir.absoluteFilePath("data"),
    newDataDir.absoluteFilePath("data"))) {
    QMessageBox::information(
      this->mainWindow,
      tr("Data directory moved"),
      tr("The data directory was successfully moved"));
    currentDataDirectory = newDataDir;
  } else {
    QMessageBox::warning(
      this->mainWindow,
      tr("Data directory not moved"),
      tr("The data directory could not be moved"));

    // Set the data directory to the original value
    QVariant dataDirVariant;
    dataDirVariant.setValue<QDir>(dataDir);
    dataDirectory->set(dataDirVariant);
  }

  dataDirectoryChangeInProgress = false;
}

void Updraft::coreSettings() {
  settingsManager->addGroup(
    "general", tr("General"), ":/core/icons/general.png");

  QDir dataDir = QCoreApplication::applicationDirPath();
  QVariant dataDirVariant;
  dataDirVariant.setValue(dataDir);
  dataDirectory = settingsManager->addSetting(
    "general:dataDir",
    tr("Data directory"),
    dataDirVariant);

  currentDataDirectory = dataDirectory->get().value<QDir>();
  dataDirectory->setNeedsRestart(true);
  dataDirectory->callOnValueChanged(this, SLOT(dataDirectoryChanged()));

  QVariant ellipsoidVariant;
  ellipsoidVariant.setValue(
    EllipsoidName(
      Util::Ellipsoid::getEllipsoidTypeName(ellipsoids.first()->getType())));
  usedEllipsoid = settingsManager->addSetting(
    "general:ellipsoid",
    tr("Ellipsoid model"),
    ellipsoidVariant);
  usedEllipsoid->setNeedsRestart(true);
}

void Updraft::createEllipsoids() {
  ellipsoids.append(
    new Util::Ellipsoid(tr("WGS84 Ellipsoid"), Util::ELLIPSOID_WGS84));
  ellipsoids.append(
    new Util::Ellipsoid(tr("FAI Sphere"), Util::ELLIPSOID_FAI_SPHERE));
}

/// Pull the lever.
/// Shows main window, and enters event loop.
int Updraft::exec() {
  mainWindow->showMaximized();
  hideSplash();
  return QApplication::exec();
}

void Updraft::hideSplash() {
  splash.finish(mainWindow);
}

}  // End namespace Core
}  // End namespace Updraft

