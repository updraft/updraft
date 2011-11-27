class IPlugin{
public:
	QString getName();

	/// Called after the plugin is loaded.
	void initialize(CoreInterface *) = 0;

	/// ... before it's destroyed.
	void deinitialize() = 0;

	/// Give a file to this plugin to open.
	/// \param out What we know about the file.
	/// \return Was opening successful?
	bool fileOpen(QString filename, struct metadata *out) = 0;

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

class CoreInterface{
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
	/// \param id serves as where for other menu creating methods.
	void createMenu(QString id, QString name);
	
	/// Adds an action to menu bar or context menu.
	/// \param where Identifier of the menu ("Core.File", "Map.Context", ...)
	/// If the menu doesn't exist something bad happens (exception?)
	void addMenuItem(QAction *, QString where, int pos);


	/// Same as createMenu, only for layer groups.
	void createLayerGroup(QString id, QString name);

	/// Add map layer.
	/// \see addMenuItem
	MapLayer *addMapLayer(osg::Node *, QString where, QString name);

	/// Open a Tab with anything.
	/// Closing is a method of class Tab.
	Tab *openTab(QWidget *, QString int pos);

	/// This plugin knows how to open the file!
	void registerFiletype(QString extension, QString description, bool persistent);

	/// Add a translator that knows how to handle strings in this plugin.
	/// Equivalent to QApplication::instance()->instalTranslator(...)
	void installTranslator(QTranslator *);
	
	/// Dependencies between plugins use this:
	/// \{
	IPlugin *getPlugin(QString name);
	QVector<IPlugin *> getAllPlugins();
	/// \}

	/// Request activation of this plugin.
	void activate();

	/// Put some text to the status widget.
	void addStatusHTML(QString html);
};
