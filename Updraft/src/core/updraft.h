#ifndef UPDRAFT_SRC_CORE_UPDRAFT_H_
#define UPDRAFT_SRC_CORE_UPDRAFT_H_

#include <QApplication>

#include "splashscreen.h"

/// Pointer to the instance of Updraft object.
#define updraft (static_cast<Updraft*>(Updraft::instance()))

class QDir;

namespace Updraft {

class SettingInterface;

namespace Util {
  class Ellipsoid;
}

namespace Core {

class MainWindow;
class FileTypeManager;
class PluginManager;
class SceneManager;
class SettingsManager;
class TranslationManager;

/// Top level object of updraft project.
class Updraft : public QApplication {
  Q_OBJECT

 public:
  Updraft(int argc, char** argv);
  ~Updraft();

  int exec();

  /// Hides the splash screen when the application is done loading.
  void hideSplash();

  // TODO(Tom): Load data directory from settings.
  // TODO(Kuba): Return QDir instead of QString
  QString getDataDirectory();
  QDir getTranslationDirectory();

  QDir currentDataDirectory;
  SettingInterface* dataDirectory;

  MainWindow* mainWindow;
  FileTypeManager* fileTypeManager;
  PluginManager* pluginManager;
  SceneManager* sceneManager;
  SettingsManager* settingsManager;
  TranslationManager* translationManager;

  // Returns the currently set ellipsoid for measurements.
  Util::Ellipsoid* getUsedEllipsoid();

  // List of ellipsoid models used in Updraft.
  QList<Util::Ellipsoid*> ellipsoids;

  SettingInterface* usedEllipsoid;

 private slots:
  void dataDirectoryChanged();

 private:
  void coreSettings();
  void createEllipsoids();

  SplashScreen splash;

  bool dataDirectoryChangeInProgress;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UPDRAFT_H_

