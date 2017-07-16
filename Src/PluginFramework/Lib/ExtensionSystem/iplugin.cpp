#include "iplugin.h"

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
