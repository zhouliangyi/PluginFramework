#include "iplugin.h"
#include "pluginmanager.h"

ExtensionSystem::IPlugin::IPlugin(QObject *parent)
{

}

ExtensionSystem::IPlugin::~IPlugin()
{

}

bool ExtensionSystem::IPlugin::initialize(const QStringList &arguments, QString &errorString)
{
    return false;
}

bool ExtensionSystem::IPlugin::addObject(QObject *obj)
{
    return false;
}

bool ExtensionSystem::IPlugin::addAutoReleasedObject(QObject *obj)
{
    return false;
}

bool ExtensionSystem::IPlugin::removeObject(QObject *obj)
{
    return false;
}

ExtensionSystem::PluginManager *ExtensionSystem::IPlugin::getManager() const
{
    return manager;
}

void ExtensionSystem::IPlugin::setManager(ExtensionSystem::PluginManager *value)
{
    manager = value;
}

ExtensionSystem::PluginSpec *ExtensionSystem::IPlugin::getPluginSpec() const
{
    return pluginSpec;
}

void ExtensionSystem::IPlugin::setPluginSpec(PluginSpec * const value)
{
    pluginSpec = value;
}
