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

  bool moveSuccessful =
    dataDir.rename(
      dataDir.absoluteFilePath("data"),
      newDataDir.absoluteFilePath("data"));

  if (!moveSuccessful) {
    moveSuccessful = moveDataDirectory(dataDir, newDataDir);
  }

  if (moveSuccessful) {
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

bool Updraft::moveDataDirectory(QDir oldDir, QDir newDir) {
  // We cannot move the directory if the destination already exists
  if (newDir.exists("data")) {
    return false;
  }

  oldDir.makeAbsolute();
  newDir.makeAbsolute();

  // Try to create the destination directory
  if (!newDir.mkdir(newDir.filePath("data")))
      return false;

  // Perform a depth-first search through the data directory and copy it
  QStack<QDir> dfsStack;

  QDir begin = oldDir;
  begin.cd("data");
  dfsStack.push(begin);

  while (!dfsStack.isEmpty()) {
    QDir srcDir = dfsStack.pop();

    foreach(
      const QFileInfo &info,
      srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
      if (info.isDir()) {
        dfsStack.push(info.absoluteFilePath());
      } else if (info.isFile()) {
        QString srcPath = info.absoluteFilePath();
        QString relFilePath = oldDir.relativeFilePath(srcPath);
        QString dstPath = newDir.filePath(relFilePath);
        if (!QFile::copy(srcPath, dstPath)) {
          qDebug() << "Data directory copying failed!";
          return false;
        }
      } else {
        qDebug() << "Warning: " << info.filePath() << " was not copied!";
      }
    }
  }

  // If we got here, everything is OK, so we can delete the old directory
  dfsStack.push(begin);
  while (!dfsStack.isEmpty()) {
    QDir srcDir = dfsStack.top();
    QFileInfoList files =
      srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    // If the directory was emptied already, delete it and remove from stack
    if (files.empty()) {
      srcDir.rmdir(srcDir.absolutePath());
      dfsStack.pop();
      continue;
    }

    // Otherwise, remove all the files and directories in the directory
    foreach(const QFileInfo &info, files) {
      if (info.isDir()) {
        dfsStack.push(info.absoluteFilePath());
      } else {
        if (!QFile::remove(info.absoluteFilePath())) {
          qDebug() << "Old data directory could not be removed!";
          return false;
        }
      }
    }
  }

  return true;
}

}  // End namespace Core
}  // End namespace Updraft

