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
class StateSaver;

/// Top level object of updraft project.
class Updraft : public QApplication {
  Q_OBJECT

 public:
  Updraft(int argc, char** argv);
  ~Updraft();

  /// Executes the application.
  /// \return The return value of the QApplication
  int exec();

  /// Hides the splash screen when the application is done loading.
  void hideSplash();

  /// Gets the data directory. This directory is saved in settings and
  /// its default location is next to the settings file.
  /// \return The application data directory.
  QDir getDataDirectory();

  /// Gets the resources directory. The resources directory is a directory
  /// with permanent data that should not be overwritten. It is stored next
  /// to the executable file.
  /// \return The resources directory
  QDir getResourcesDirectory();

  /// Gets the directory where the translations are located.
  /// \return The translation directory
  QDir getTranslationDirectory();

  /// Ask all plug-ins if closing them is Ok.
  /// \see PluginBase::askClose()
  bool askClose();

  /// Reimplemented from QApplication.
  /// Handles behavior on system shutdown.
  /// If user interaction is allowed then asks all plug-ins what to do
  /// and possibly cancels the shutdown.
  void commitData(QSessionManager& sessionManager);  // NOLINT

  QDir currentDataDirectory;
  SettingInterface* dataDirectory;

  MainWindow* mainWindow;
  FileTypeManager* fileTypeManager;
  PluginManager* pluginManager;
  SceneManager* sceneManager;
  SettingsManager* settingsManager;
  TranslationManager* translationManager;
  StateSaver* stateSaver;

  // Returns the currently set ellipsoid for measurements.
  Util::Ellipsoid* getUsedEllipsoid();

  // List of ellipsoid models used in Updraft.
  QList<Util::Ellipsoid*> ellipsoids;

  SettingInterface* usedEllipsoid;

 private slots:
  void dataDirectoryChanged();

 private:
  void coreSettings();

  /// Checks and possibly creates the data directory.
  /// \return Whether the data directory exists or it was successfully created
  bool checkDataDirectory();

  /// Initializes the ellipsoids used for the measurements inside the
  /// application.
  void createEllipsoids();

  SplashScreen splash;

  bool dataDirectoryChangeInProgress;

  /// An auxilliary method for directory copying.
  /// If the destination directory exists, the method fails.
  /// \param srcDir The source directory of the copying.
  /// \param dstDir The destination directory.
  /// \param progress Pointer to an integer variable that should be increased
  ///                 every time a file is copied. Can be NULL to disable the
  ///                 progressbar feature.
  /// \param dialog Pointer to the progress dialog that displays the progress
  ///               stored in the progress parameter. It can be NULL, in which
  ///               case no progress dialog is shown.
  /// \return True when the copying succeeds.
  bool copyDirectory(
    QDir srcDir,
    QDir dstDir,
    int* progress = NULL,
    QProgressDialog* dialog = NULL);

  /// Moves the data directory from one location to another. This method first
  /// tries to rename the directory and if that fails (e.g. because the source
  /// and destination directory would be on different disk partitions), it
  /// tries to do a copy using the copyDirectory() method. If the copying
  /// succeeds, the data directory setting is updated and the old data
  /// directory is deleted.
  /// \param oldDir The source data directory that should be copied.
  /// \param newDir The QDir object with the name of the new data directory.
  ///               It should not exist, otherwise, the movement fails.
  /// \param dialog Poiter to a progress dialog that will be shown, should the
  ///               copying take too long.
  /// \return True when the movement succeeds.
  bool moveDataDirectory(QDir oldDir, QDir newDir, QProgressDialog* dialog);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UPDRAFT_H_

