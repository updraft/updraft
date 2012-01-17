#ifndef UPDRAFT_SRC_FILEREGISTRATION_H_
#define UPDRAFT_SRC_FILEREGISTRATION_H_

namespace Updraft {

enum FileCategory {
  CATEGORY_PERSISTENT = 1,
  CATEGORY_TEMPORARY = 2
};

class PluginBase;

struct FileRegistration {
  /// File extension
  QString extension;

  /// Description of files with specified extension
  QString typeDescription;

  /// Id specified by plug-in (each plug-in has it's own namespace).
  /// It is used for identification of selected role.
  int roleId;

  /// Role description shown in Open dialog
  QString roleDescription;

  /// Temporary or imported(persistent) file
  FileCategory category;

  /// Destination subdirectory for imported file
  /// This subdirectory is placed in application data directory.
  /// If category is CATEGORY_TEMPORARY, importDirectory is ignored.
  /// If importDirectory is empty, files are copied directly to data directory.
  QString importDirectory;

  /// Pointer to associated plugin
  /// This plugin handles file opening.
  PluginBase *plugin;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_FILEREGISTRATION_H_
