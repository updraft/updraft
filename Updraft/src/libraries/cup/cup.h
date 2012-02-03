#ifndef UPDRAFT_SRC_LIBRARIES_CUP_CUP_H_
#define UPDRAFT_SRC_LIBRARIES_CUP_CUP_H_

#include <QtCore/QtGlobal>
#include <QString>
#include <QList>
#include "../util/util.h"

#ifdef CUP_LIB
# define CUP_EXPORT Q_DECL_EXPORT
#else
# define CUP_EXPORT Q_DECL_IMPORT
#endif

namespace Updraft {
namespace Cup {

class CupLoader;

/// Structure defining 'cup style' turn-point
/// This type exactly copies the data from cup file.
// TODO(Tom): Format values (e.g. remove double quotes)
struct CUP_EXPORT TPEntry {
  QString name;
  QString code;
  QString country;
  Util::Location location;
  QString style;
  QString rwyDirection;
  QString rwLength;
  QString frequency;
  QString description;
};

/// Structure defining 'cup style' task
// TODO(Tom): Think wheter to load the tasks from cup.
struct CUP_EXPORT TaskEntry {
};

/// Stores and encapsulates data of the SeeYou cup file.
/// Cup file is loaded by the CupLoader.
/// Provides access to the loaded data.
class CUP_EXPORT CupFile {
 public:
  // TODO(Tom): Define interface

  /// \return A file name with full path.
  QString getFileName() const;

  /// \return A list of all turn-points.
  QList<TPEntry> getTPEntries() const;

 private:
  QString fileName;

  // TODO(Tom): Consider the best container type (e.g. map)
  QList<TPEntry> turnPoints;
  QList<TaskEntry> tasks;

  /// The new instance can be created only by CupLoader.
  CupFile() {}

  /// Sets the file name.
  /// Used by CupLoader during building process.
  /// \param name a cup file name (with full path)
  void setFileName(const QString &name);

  /// Adds turn-point.
  /// Used by CupLoader during building process.
  /// \param tp a reference to the struct containing tp data
  void addTPEntry(const TPEntry &tp);

  /// Adds task
  /// Used by CupLoader during building process.
  /// \param task a reference to the struct containing task data
  void addTaskEntry(const TaskEntry &task);

  friend class CupLoader;
};

/// Performs loading of SeeYou cup files.
/// Creates CupFile instances.
class CUP_EXPORT CupLoader {
 public:
  CupLoader();

  /// Loads file from disk.
  /// \param name a name of the file (with full path)
  /// \return Pointer to the new CupFile instance
  CupFile* loadFile(const QString &name);

 protected:
  /// Specifies inner state of the loading process.
  enum LoadingState {
    LOADING_HEADER,
    LOADING_TPS,
    LOADING_TASKS,
    UNDEFINED,
    DONE,
    ERROR
  };

  LoadingState state;   // Inner state of the loading process
  CupFile *cupFile;     // Instance of the currently built file

  /// Parses one line stored in QByteArray.
  /// \param strLine a string storing one line of the file
  void parseLine(const QString &strLine);

  /// Parses latitude entry.
  /// \param tp destination instance of TPEntry
  /// \param latitude a string storing latitude in cup format
  void parseLatitude(TPEntry &tp, const QString &latitude);

  /// Parses longitude entry.
  /// \param tp destination instance of TPEntry
  /// \param longitude a string storing longitude in cup format
  void parseLongitude(TPEntry &tp, const QString &longitude);

  /// Parses elevation entry. Supports both feets and meters.
  /// \param tp destination instance of TPEntry
  /// \param elevation a string storing elevation in cup format
  void parseElevation(TPEntry &tp, const QString &elevation);

  /// Parses line containing turn-point definition.
  /// \param strLine a string storing one line of the file
  void parseTP(const QString &strLine);

  /// Parses line containing task declaration.
  /// \param strLine a string storing one line of the file
  void parseTask(const QString &strLine);
};

}  // End namespace Cup
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_CUP_CUP_H_
