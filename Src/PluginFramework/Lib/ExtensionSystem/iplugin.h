#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
namespace ExtensionSystem{

class PluginManager;
class PluginSpec;

class IPlugin : public QObject
{
    Q_OBJECT
public:
    explicit IPlugin(QObject *parent = nullptr);

    virtual ~IPlugin();
    virtual bool initialize(const QStringList &arguments,QString &errorString);
    virtual bool initializeExtensions(const QStringList &arguments,QString &errorString);

    //    virtual void extensionInitialized() = 0;


    // 添加对象到内存池中
    bool addObject(QObject *obj);
    bool addAutoReleasedObject(QObject *obj);
    bool removeObject(QObject *obj);

    PluginManager *getManager() const;
    void setManager(PluginManager *value);

    PluginSpec *getPluginSpec() const;
    void setPluginSpec(PluginSpec * const value);

private:
    PluginSpec * pluginSpec;
    // 逆序存放对象 在析构时自动释放所占内存
    QObjectList addedObjectsInReverseOrder;
    PluginManager* manager; //保存pluginManager的引用
};

}

#endif // IPLUGIN_H
