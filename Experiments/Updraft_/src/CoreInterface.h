#ifndef COREINTERFACE_H
#define COREINTERFACE_H

class CoreInterface{
public:

    //osgViewer::View* getOsgView();
    //osg::Group* getOsgRoot();
    // event handling is done through getOsgView()->addEventHandler
    // catching rendered objects

    void addMenu(QMenu *, QString id, int pos);

    void registerContextMenu(QMenu *, QString id);

    /// Adds an action to menu bar or context menu.
    /// \param where Identifier of the menu ("Core.File", "Task.Context", ...)
    void addMenuItem(QAction *, QString where, int pos);

    QMenu *getMenu(QString id);

    ToolPanel *addToolPanel(QWidget *, int pos);

    PluginInterface *getPlugin(QString name);
    // TODO: Package access
};

#endif // COREINTERFACE_H
