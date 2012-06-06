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
#include "statesaver.h"

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

  // Initialize the data directory if needed
  if (!checkDataDirectory()) {
    qDebug() << "The data directory is invalid and could not be restored";
  }

  mainWindow = new MainWindow(NULL);
  fileTypeManager = new FileTypeManager();
  sceneManager = new SceneManager();

  stateSaver = new StateSaver();

  mainWindow->setMapWidget(sceneManager->getWidget());

  pluginManager = new PluginManager();

  QStringList args = arguments();
  for (int i = 1; i < args.count(); ++i) {
    fileTypeManager->openFile(args[i]);
  }

  // Turn on the data directory slot
  dataDirectory->callOnValueChanged(this, SLOT(dataDirectoryChanged()));
  dataDirectoryChangeInProgress = false;
}

Updraft::~Updraft() {
  QByteArray x = StateSaver::saveViewpoint(
    sceneManager->getViewpoint());

  qDebug() << x;
  qDebug() << StateSaver::saveViewpoint(StateSaver::restoreViewpoint(x));

  stateSaver->save();
  delete stateSaver;

  delete sceneManager;
  delete pluginManager;
  delete fileTypeManager;
  delete settingsManager;
  delete mainWindow;

  foreach(Util::Ellipsoid *e, ellipsoids) {
    delete e;
  }
}

QDir Updraft::getDataDirectory() {
  QDir dataDir = currentDataDirectory;
  dataDir.cd("data");
  dataDir.makeAbsolute();
  qDebug() << "getDataDirectory" << dataDir.path();
  return dataDir;
}

QDir Updraft::getResourcesDirectory() {
  QDir dataDir(applicationDirPath());
  dataDir.cd("resources");
  return dataDir;
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

  sceneManager->destroyMaps();
  bool moveSuccessful =
    dataDir.rename(
      dataDir.absoluteFilePath("data"),
      newDataDir.absoluteFilePath("data"));

  // Moving failed we will try to copy
  if (!moveSuccessful) {
    QProgressDialog progress(tr("Counting files..."), "", 0, 100, mainWindow);
    progress.setCancelButton(0);
    progress.setWindowModality(Qt::WindowModal);

    moveSuccessful = moveDataDirectory(
      dataDir.absoluteFilePath("data"),
      newDataDir.absoluteFilePath("data"),
      &progress);
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

  sceneManager->createMaps();
  dataDirectoryChangeInProgress = false;
}

void Updraft::coreSettings() {
  settingsManager->addGroup(
    "general", tr("General"), GROUP_VISIBLE, ":/core/icons/general.png");

  QDir dataDir = settingsManager->getSettingsDir();
  QVariant dataDirVariant;
  dataDirVariant.setValue(dataDir);
  dataDirectory = settingsManager->addSetting(
    "general:dataDir",
    tr("Data directory"),
    dataDirVariant);

  currentDataDirectory = dataDirectory->get().value<QDir>();
  qDebug() << "Current data directory: " << currentDataDirectory;

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

bool Updraft::checkDataDirectory() {
  // First try if the data directory exists
  QDir dataDir = currentDataDirectory;
  qDebug() << "Data directory found in settings: " << dataDir;
  if (dataDir.cd("data")) {
    qDebug() << "Data directory correct in settings";
    return true;
  }

  // Restore data directory to default location (next to settings)
  QDir defaultDataDir = QDir(settingsManager->getSettingsDir());

  QVariant dirVariant;
  dirVariant.setValue<QDir>(defaultDataDir);
  dataDirectory->set(dirVariant);

  currentDataDirectory = defaultDataDir;

  // Check if data directory exists at default location
  if (dataDir.exists("data")) {
    qDebug() << "Data directory found next to settings";
    return true;
  }

  // If the data directory is not valid, create it by settings.xml
  QDir srcDir = getResourcesDirectory();
  srcDir.cd("data");
  if (copyDirectory(srcDir, defaultDataDir.absoluteFilePath("data"))) {
    qDebug() << "Data directory created next to settings";
    return true;
  }
  return false;
}

/// Pull the lever.
/// Shows main window, and enters event loop.
int Updraft::exec() {
  mainWindow->show();

  stateSaver->load();

  hideSplash();
  return QApplication::exec();
}

void Updraft::hideSplash() {
  splash.finish(mainWindow);
}

bool Updraft::copyDirectory(
  QDir srcDir,
  QDir dstDir,
  int* progress,
  QProgressDialog* dialog) {
  // Set the dialog label
  if (dialog)
    dialog->setLabelText(QString(tr("Copying directory %1 to %2...")).
    arg(srcDir.absolutePath()).arg(dstDir.absolutePath()));

  QStack<QDir> dfsStack;

  // Begin with the copied directory
  dfsStack.push(srcDir);

  // Try to create the destination directory
  if (dstDir.exists())
      return false;

  dstDir.mkpath(".");

  // Take directories from the stack and copy them
  while (!dfsStack.isEmpty()) {
    QDir dir = dfsStack.pop();

    foreach(
      const QFileInfo &info,
      dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
      // Prepare the variables
      QString srcPath = info.absoluteFilePath();
      QString relFilePath = srcDir.relativeFilePath(srcPath);
      QString dstPath = dstDir.filePath(relFilePath);

      // Copy files and directories
      if (info.isDir()) {
        dfsStack.push(info.absoluteFilePath());
        if (!dstDir.mkpath(relFilePath)) {
          qDebug() << "Directory copying failed! Could not create" <<
            dstPath;
          return false;
        }
      } else if (info.isFile()) {
        if (!QFile::copy(srcPath, dstPath)) {
          qDebug() << "Directory copying failed! Could not copy " <<
            srcPath << " to " << dstPath;
          return false;
        }

        // If both the progress indicator and progress dialog contain valid
        // pointers, update them.
        if (progress && dialog) {
          (*progress)++;
          if ((*progress) % 100 == 0)
            dialog->setValue(*progress);
        }
      } else {
        qDebug() << "Warning: " << info.filePath() << " was not copied!";
      }
    }
  }

  return true;
}

bool Updraft::moveDataDirectory(
  QDir oldDir,
  QDir newDir,
  QProgressDialog* dialog) {
  // We cannot move the directory if the destination already exists
  if (newDir.exists()) {
    return false;
  }

  oldDir.makeAbsolute();
  newDir.makeAbsolute();

  // The directory traversal will be done using DFS
  QStack<QDir> dfsStack;

  // Count the total number of files
  int filenum = 0;
  dfsStack.push(oldDir);
  while (!dfsStack.isEmpty()) {
    QDir srcDir = dfsStack.pop();

    foreach(
      const QFileInfo &info,
      srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
      if (info.isDir()) {
        dfsStack.push(info.absoluteFilePath());
      } else {
        filenum++;  // Only count files
      }
    }
  }

  // Set the progressbar range to double the number of files (copy and move)
  dialog->setRange(0, filenum*2);
  int progress = 0;

  // Copy the data directory
  bool success = copyDirectory(oldDir, newDir, &progress, dialog);
  if (!success) {
    dialog->setValue(filenum*2);  // Dismiss the dialog
    return false;
  }

  // If we got here, everything is OK, so we can delete the old directory
  dialog->setLabelText(tr("Removing old data directory..."));

  dfsStack.push(oldDir);
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
          dialog->setValue(filenum*2);
          // todo: show message that old directory could not be deleted
          // return false;
        }

        progress++;
        if (progress % 100 == 0)
          dialog->setValue(progress);
      }
    }
  }

  dialog->setValue(filenum*2);
  return true;
}

}  // End namespace Core
}  // End namespace Updraft

