#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

class PluginInterface
{
public:
    PluginInterface();
    virtual ~PluginInterface() = NULL;

    virtual QString getName() = NULL;

        /// Called after the plugin is loaded.
    virtual void initialize(CoreInterface *) = NULL;
    virtual void deinitialize() = NULL;

    virtual bool fileOpened(QString filename) = NULL;
    virtual bool fileOpened(istream stream) = NULL;
};

#endif // PLUGININTERFACE_H
