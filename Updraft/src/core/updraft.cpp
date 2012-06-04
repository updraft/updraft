#include "updraft.h"

#include <QDesktopServices>

#include "../maplayerinterface.h"
#include "../settinginterface.h"
#include "ui/maplayergroup.h"
#include "util/util.h"
#include "variantfunctions.h"
#include "ui/mainwindow.h"
#include "filetypemanager.h"
#include "pluginmanager.h"
#include "scenemanager.h"
#include "settingsmanager.h"
#include "translationmanager.h"

namespace Updraft {
namespace Core {

Updraft::Updraft(int argc, char** argv)
  : QApplication(argc, argv) {
  splash.show();

  // Needed so that we can use custom types in QVariant
  registerMetaTypes();

  settingsManager = new SettingsManager();
  translationManager = new TranslationManager();

  coreSettings();
  createEllipsoids();

  mainWindow = new MainWindow(NULL);
  fileTypeManager = new FileTypeManager();
  sceneManager = new SceneManager();

  mainWindow->setMapWidget(sceneManager->getWidget());

  pluginManager = new PluginManager();

  QStringList args = arguments();
  for (int i = 1; i < args.count(); ++i) {
    fileTypeManager->openFile(args[i]);
  }
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
  QDir dataDir = dataDirectory->get().value<QDir>();
  return dataDir.absolutePath();
}

QDir Updraft::getTranslationDirectory() {
  return QDir(applicationDirPath());
}

void Updraft::dataDirectoryChanged() {
  QMessageBox::information(
    this->mainWindow,
    tr("Information"),
    tr("Change to data directory won't take effect until the next restart."));
}

void Updraft::coreSettings() {
  settingsManager->addGroup(
    "general", tr("General"), ":/core/icons/general.png");

  QDir dataDir = QCoreApplication::applicationDirPath();
  dataDir.cd("data");
  QVariant dataDirVariant;
  dataDirVariant.setValue(dataDir);
  dataDirectory = settingsManager->addSetting(
    "general:dataDir",
    tr("Data directory"),
    dataDirVariant);

  currentDataDirectory = dataDirectory->get().value<QDir>();
  dataDirectory->setNeedsRestart(true);
  dataDirectory->callOnValueChanged(this, SLOT(dataDirectoryChanged()));
}

void Updraft::createEllipsoids() {
  ellipsoids.append(new Util::Ellipsoid(tr("WGS84 Ellipsoid"),
    Util::Units::WGS84EquatRadius(), Util::Units::WGS84Flattening()));
  ellipsoids.append(new Util::Ellipsoid(tr("FAI Sphere"),
    Util::Units::FAISphereRadius()));
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

