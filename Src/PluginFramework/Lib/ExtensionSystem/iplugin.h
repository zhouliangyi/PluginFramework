#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
namespace ExtensionSystem{
namespace Internal {
class PluginSpec;

}
class IPlugin : public QObject
{
    Q_OBJECT
public:
    explicit IPlugin(QObject *parent = nullptr);

    virtual ~IPlugin();
    virtual bool initialize(const QStringList &arguments,QString &errorString);

    //    virtual void extensionInitialized() = 0;


    // 添加对象到内存池中
    bool addObject(QObject *obj);
    bool addAutoReleasedObject(QObject *obj);
    bool removeObject(QObject *obj);
private:
    Internal::PluginSpec *pluginSpec;
    // 逆序存放对象 在析构时自动释放所占内存
    QObjectList addedObjectsInReverseOrder;
};

}

#endif // IPLUGIN_H
