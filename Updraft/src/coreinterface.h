#ifndef UPDRAFT_CORE_INTERFACE_H_
#define UPDRAFT_CORE_INTERFACE_H_

#include "core/ui/menu.h"
#include "core/ui/tab.h"

class QWidget;
class QMainWindow;

namespace Updraft {

enum SystemMenu {
  MENU_FILE,
  MENU_EDIT,
  MENU_TOOLS,
  MENU_HELP,
  MENU_CONTEXT
};

enum FileCategory {
  CATEGORY_PERSISTENT,
  CATEGORY_TEMPORARY
};

/// Exposes core functionalities to plugins.
/// A call to methods of this interface automagically contains
/// a pointer to calling plugin (this is ensured in core/coreimplementation.cpp)
class CoreInterface {
 public:
  /// Returns main window pointer. Used for example for modal dialogs
  virtual QMainWindow* getMainWindow() = 0;

  /// Create an entry in the main menu.
  virtual Core::Menu* createMenu(QString title) = 0;

  /// Remove an entry from the main menu
  virtual void removeMenu(Core::Menu* menu) = 0;

  /// Returns pointer to the instance of a system menu
  /// \param menu which system menu instance to return  
  virtual Core::Menu* getSystemMenu(SystemMenu menu) = 0;

  /// Open a Tab with any widget.
  /// Takes ownership of content.
  virtual Core::Tab* createTab(QWidget* content, QString title) = 0;

  /// Removes tab from the bottom pane
  /// The tab class given to this function is deleted!
  virtual void removeTab(Core::Tab* tab) = 0;
  
  /// This plugin knows how to open the file!
  /// \return Identifier of the newly registered filetype
  virtual int registerFiletype(QString extension, QString description,
    FileCategory category) = 0;

  /// Unregisters a given file type
  virtual void unregisterFileType(int id) = 0;
};

}  // End namespace Updraft

#endif

