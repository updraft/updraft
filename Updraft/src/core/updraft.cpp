#include "updraft.h"

#include <QDesktopServices>

#include "../maplayerinterface.h"
#include "../settinginterface.h"
#include "ui/maplayergroup.h"
#include "util/util.h"
#include "variantfunctions.h"

namespace Updraft {
namespace Core {

Updraft::Updraft(int argc, char** argv)
  : QApplication(argc, argv) {
  // Needed so that we can use custom types in QVariant
  registerMetaTypes();

  QTranslator trans;
  trans.load("translations/czech");

  installTranslator(&trans);

  splash.show();

  mainWindow = new MainWindow(NULL);

  settingsManager = new SettingsManager();
  settingsManager->finishInit();
  QDir dataDir = QCoreApplication::applicationDirPath();
  dataDir.cd("data");
  QVariant dataDirVariant;
  dataDirVariant.setValue(dataDir);
  dataDirectory = settingsManager->addSetting(
    "core:dataDir",
    "Data directory",
    dataDirVariant);

  fileTypeManager = new FileTypeManager();
  sceneManager = new SceneManager();

  // Initializes list of available ellipsoids.
  ellipsoids.append(new Util::Ellipsoid(tr("WGS84"),
    Util::Units::WGS84EquatRadius(), Util::Units::WGS84Flattening()));
  ellipsoids.append(new Util::Ellipsoid(tr("FAI Sphere"),
    Util::Units::FAISphereRadius()));

  pluginManager = new PluginManager();
  pluginManager->finishInit();

  mainWindow->setMapWidget(sceneManager->getWidget());
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

