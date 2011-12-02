class IPlugin {
 public:
  /// Plugin metadata
  /// \{
  virtual QString getName() = 0;

  /// Priority gives the order in which plugins receive callbacks
  virtual unsigned getPriority() = 0;

  /// Plugin's API version.
  virtual unsigned getPluginApiVersion() { return PLUGIN_API_VERSION }
  /// \}

  /// Called after the plugin is loaded
  virtual void initialize(CoreInterface *coreInterface) = 0;

  /// ... before it's destroyed.
  virtual void deinitialize() {}

  /// Give a file to this plugin to open
  /// This callback is sent to the file
  /// \return Was opening successful?
  virtual bool fileOpen(QString filename) { return false; }

  /// Callbacks when a registered map layer is displayed or hidden in left pane.
  /// \{
  void mapLayerHidden(osg::Node* layer) {}
  void mapLayerDisplayed(osg::Node* layer) {}
  /// \}

  void tabSelected(Tab *tab) {}
  void tabDeselected(Tab *tab) {}
  void tabClosed(Tab *tab) {}

  /// Main window lost focus.
  void focusLost() {}

  /// Click, double click, drag drop, ...
  void mapEvent(Event *e) {}

  /// Any plugin setting was changed.
  void settingChanged() {}
};

class Menu: public QMenu {
  Q_OBJECT

 public:
    void insertAction(int pos, QAction* action);
};

class MapLayerGroup {
 public:
  void insertMapLayer(int pos, osg::Node* layer, QString title);
  void removeMapLayer(osg::Node* layer);
};

enum SystemMenu {
  MENU_FILE,
  MENU_EDIT,
  MENU_TOOLS,
  MENU_HELP,
  MENU_CONTEXT
};

enum SystemMapLayerGroup {
  /// terrain, height map, country boundaries...
  MAP_LAYER_GROUP_BASE
}

/// Says whether a file will be interpreted as a new data source to import or as
/// a new file to temporarily open.
enum FilePersistence {
  PERSIST_IMPORT,
  PERSIST_TEMPORARY
};

class Viewer: public osgViewer::Viewer {
 public:
  /// Work with the camera transformation
  /// \{
  Transform getCameraTransform();
  void setCameraTransform(Transform transformation);
  /// \}
};

/// Class representing a single setting -- user or internal.
template <typename T>
class Setting {
 public:
  T get();
  void set(T value);

  bool isDefault();
 
 private:
  /// Edit widget edits any kind of data we migt need,
  /// including unit conversions.
  QWidget *getEditWidget();

  friend class SettingsWindow;
};

/// Version that is incremented every time the plugin API or ABI changes.
/// Used to check if plugin will be usable with the core.
#define PLUGIN_API_VERSION 0

class CoreInterface {
 private:
  /// Private constructor, usable only by plugin manager.
  /// Every plugin gets its own instance of core interface.
  explicit CoreInterface(IPlugin* pl): plugin(pl) {}

  /// The plugin this interface works with
  IPlugin* plugin;

 public:
  /// Manipulation with OSG node tree and view.
  /// \{
  //// Viewer is a subclass of osgViewer::View
  Viewer *getViewer();
  osg::Group* getOsgRoot();
  /// \}

  /// Create an entry in the main menu.
  Menu* createMenu(QString title);

  /// Remove an entry from the main menu
  void removeMenu(Menu* menu);

  /// Returns pointer to the instance of a system menu
  /// \param menu which system menu instance to return
  Menu* getSystemMenu(SystemMenu menu);

  /// Same as createMenu, only for layer groups.
  MapLayerGroup* createMapLayerGroup(QString title);

  /// Removes a layer group from the left pane
  void removeMapLayerGroup(MapLayerGroup* group);

  MapLayerGroup *getSystemMapLayerGroup(SystemMapLayerGroup group);

  /// Open a Tab with anything.
  /// Closing is a method of class Tab.
  Tab* createTab(QWidget* content, QString title);

  /// Removes tab from the bottom pane
  void removeTab(Tab* tab);

  /// Put some text to the info widget.
  void addInfoHTML(QString html);

  /// This plugin knows how to open the file!
  /// \param persist Says whether the file will be interpreted as a new data
  /// source to import or as a new file to temporarily open.
  /// \return Identifier of the newly registered filetype
  int registerFiletype(QString extension, QString description,
    FilePersistence persist);

  /// Unregisters a given file type
  void unregisterFileType(int id);

  /// Dependencies between plugins use this:
  /// \{
  IPlugin *getPlugin(QString name);
  QVector<IPlugin *> getAllPlugins();
  /// \}

  template <typename T>
  Setting<T> registerSetting(QString key, T defaultValue);

  template <typename T>
  Setting<T> registerUserSetting(QString key, T defaultValue,
    QString title, QString description);

};
