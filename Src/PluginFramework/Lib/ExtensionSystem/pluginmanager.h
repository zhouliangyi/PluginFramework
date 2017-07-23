#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
namespace ExtensionSystem {
class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = nullptr);

signals:

public slots:
};
}


#endif // PLUGINMANAGER_H
