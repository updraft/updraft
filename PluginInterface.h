class IPlugin{
public:
	QString getName();

	/// Called after the plugin is loaded.
	void initialize(CoreInterface *) = 0;

	/// ... before it's destroyed.
	void deinitialize() = 0;

	/// Give a file to this plugin to open
	/// \return Was opening successful?
	bool fileOpen(QString filename) { return false; }

	/// Used when the plugin gains/looses the priority.
	/// \see CoreInterface::activate()
	/// \{
	void activated();
	void deactivated();
	/// \}

	/// Should be the same as CoreInterface::PLUGIN_API_VERSION.
	qint pluginApiVersion();

	/// Callbacks when a registered map layer is displayed or hidden using the checkbox in left pane.
	/// \{
	void mapLayerHidden(MapLayer *) = 0;
	void mapLayerDisplayed(MapLayer *) = 0;
	/// \}
	
	/// Click, double click, drag drop, ...
	void mapEvent(Event *);
};

class Menu: public QMenu {
Q_OBJECT

 public:
    void insertAction(int pos, QAction* action);
};

class MapLayerGroup {
 public:
  void insertMapLayer(int pos, osg::Node* layer, QString title);
    //XXX: we need to find out exactly how the hell one modifies the osg scene graph
  void removeMapLayer(osg::Node* layer);
 protected:
   //Structures that hold some information about the layers
};

enum SystemMenu {
	MENU_FILE,
	MENU_EDIT,
	MENU_CONTEXT
};

/// Says whether a file will be interpreted as a new data source to import or as
/// a new file to temporarily open.
enum FilePersistence {
	PERSIST_IMPORT,
	PERSIST_TEMPORARY
};

class CoreInterface{
 private:
        CoreInterface
 public:
	static const qint PLUGIN_API_VERSION = 0;

	// ?????????????????????????????
	osgViewer::View* getOsgView();
	osg::Group* getOsgRoot();
	// event handling is doe through getOsgView()->addEventHandler
	// catching rendered objects
	// ?????????????????????????????
	

	// I'm not sure about the 'where' arguments, but I dont have any other idea how to do it.
	
	/// Create an entry in the main menu.
	Menu* createMenu(QString title);

	/// Remove an entry from the main menu
	void removeMenu(Menu* menu);

	/// Returns pointer to the instance of a system menu
	/// \param menu which system menu instance to return
	Menu* getSystemMenu(SystemMenu menu);

	/// Same as createMenu, only for layer groups.
	MapLayerGroup* createLayerGroup(QString title);

	/// Removes a layer group from the left pane
	void removeLayerGroup(MapLayerGroup* group);

	/// Open a Tab with anything.
	/// Closing is a method of class Tab.
	Tab* createTab(QWidget* content, QString title);

	/// Removes tab from the bottom pane
	void removeTab(Tab* tab);

	/// Put some text to the info widget.
	void addInfoHTML(QString html);

	/// This plugin knows how to open the file!
	/// \param persist Says whether the file will be interpreted as a new data source to
	///                import or as a new file to temporarily open.
	/// \return Identifier of the newly registered filetype
	int registerFiletype(QString extension, QString description, FilePersistence persist);

	/// Unregisters a given file type
	void unregisterFileType(int id);

	/// Request activation of this plugin.
	void activate();

	/// Add a translator that knows how to handle strings in this plugin.
	/// Equivalent to QApplication::instance()->instalTranslator(...)
	void installTranslator(QTranslator *);
	
	/// Dependencies between plugins use this:
	/// \{
	IPlugin *getPlugin(QString name);
	QVector<IPlugin *> getAllPlugins();
	/// \}
};
