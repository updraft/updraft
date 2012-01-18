#ifndef UPDRAFT_SRC_COREINTERFACE_H_
#define UPDRAFT_SRC_COREINTERFACE_H_

#include <QString>

#include "menuinterface.h"
#include "tabinterface.h"
#include "fileregistration.h"

class QWidget;
class QMainWindow;

namespace Updraft {

class MapLayerGroup;

/// Exposes core functionalities to plugins.
/// A call to methods of this interface automagically contains
/// a pointer to calling plugin (this is ensured in core/coreimplementation.cpp)
class CoreInterface {
 public:
  virtual ~CoreInterface() {}

  /// Returns main window pointer. Used for example for modal dialogs
  virtual QMainWindow* getMainWindow() = 0;

  /// Create an entry in the main menu.
  virtual MenuInterface* createMenu(QString title) = 0;

  /// Returns pointer to the instance of a system menu
  /// \param menu which system menu instance to return
  virtual MenuInterface* getSystemMenu(SystemMenu menu) = 0;

  /// Creates map layer group and adds it to the tree view in the left pane.
  /// To remove MapLayerGroup use C++ delete.
  /// \param title label for the item in the tree view
  /// \return Pointer to the new MapLayerGroup
  virtual MapLayerGroup* createMapLayerGroup(const QString &title) = 0;

  /// Open a Tab with any widget.
  /// Takes ownership of content.
  virtual TabInterface* createTab(QWidget* content, QString title) = 0;

  /// Removes tab from the bottom pane
  /// The tab class given to this function is deleted!
  /// Equivalent to tab->close()
  virtual void removeTab(TabInterface* tab) = 0;

  /// This plugin knows how to open the file!
  /// \return Identifier of the newly registered filetype
  virtual void registerFiletype(const FileRegistration &registration) = 0;

  /// Gets path of the application data directory.
  /// \return Full path to the application directory
  virtual QString getDataDirectory() = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_COREINTERFACE_H_

