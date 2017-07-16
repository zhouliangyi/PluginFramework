#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H


#include <QString>
#include <QPluginLoader>
#include <QVector>
namespace ExtensionSystem
{
class IPlugin;

struct PluginDependency
{
    enum Type {
        Required,
        Optional,
        Test
    };

    PluginDependency() : type(Required) {}

    QString name;
    QString version;
    Type type;
    bool operator==(const PluginDependency &other) const;
    QString toString() const;
};

class PluginSpec
{
public:
    PluginSpec();

    enum State
    {
        Invalid,Read,Resolved,Loaded,Initialized,Running,Stopped,Deleted
    };


public:

    // 读插件
    bool read(const QString &fileName);
    // 解析依赖项 给定一组插件，判断当前插件的依赖项是否齐全 如果齐全则将
    bool resolveDependencies(const QList<PluginSpec*> &specs);

    bool loadLibrary();
    bool initializePlugin();
    bool initializeExtensions();
    bool delayedInitialize();
    void kill();


    // 给定插件名称和版本，判断是否满足要求
    bool provides(const QString &pluginName,const QString &version) const;


//private:
    // 读取json文件中的插件元信息
    bool readMetaData(const QJsonObject &metaData);
    int versionCompare(const QString &version1, const QString &version2) const; // 比较版本 version1 > version2 返回1，反之返回-1, 相等返回0
private:

    QPluginLoader loader;

    /////////////////////////////////////////////////////////////////
    /// \brief 下面这些都是从插件元信息中读取
    ///
    QString name; // 插件名称
    QString version;// 版本号
//    QString compatVersion;
//    bool required;
    bool experimental;
//    bool enabledByDefault;
    QString vendor;
    QString copyright;
    QString license;
    QString url;
    QString category;
//    QRegExp platformSpecification;
    QVector<PluginDependency> dependencies;
    /////////////////////////////////////////////////////////////////


    bool enabledBySettings;
    bool enabledIndirectly;
    bool forceEnabled;
    bool forceDisabled;
    QString location;
    QString filePath;
    QStringList arguments;
    QHash<PluginDependency,PluginSpec*> dependencySpecs;

    IPlugin* plugin;
    PluginSpec::State state;
    bool hasError;
    QString errorString;

};
}

#endif // PLUGINSPEC_H
