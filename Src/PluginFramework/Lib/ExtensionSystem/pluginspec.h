#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H


#include <QString>
#include <QPluginLoader>
#include <QVector>
namespace ExtensionSystem
{
class IPlugin;

/**
 * @brief The PluginDependency struct该类表示依赖信息
 */
struct PluginDependency
{
    enum Type {
        Required,/**< 必须的 */
        Optional,/**< 可选的 */
        Test/**< 测试*/
    };

    /**
     * @brief 构造函数
     *
     * @fn PluginDependency
     */
    PluginDependency() : type(Required) {}

    QString name; /**< 插件名称 */
    QString version; /**< 插件版本*/
    Type type; /**< 依赖类型 */
    /**
     * @brief 判等符号
     *
     * @fn operator ==
     * @param other
     * @return bool operator
     */
    bool operator==(const PluginDependency &other) const;
//    QString toString() const;
};
/**
 * @brief全局计算哈希值的函数，使用该类型作为QHash的key必须要有该函数
 *
 * @fn qHash
 * @param value
 * @return uint
 */
uint qHash(const ExtensionSystem::PluginDependency &value);

/**
 * @brief
 - 该类是插件的容器，包括插件的加载器和插件相关的信息
 *
 * @class PluginSpec pluginspec.h "pluginspec.h"
 */
class PluginSpec
{
public:
    /**
     * @brief构造函数
     *
     * @fn PluginSpec
     */
    PluginSpec();

    /*!
     * \brief The State enum 插件状态
     */
    enum State
    {
        Invalid,/**< 无效*/
        Read,/**< 已读*/
        Resolved,/**< 已解析*/
        Loaded,/**< 已加载*/
        Initialized,/**< 已初始化*/
        Running,/**< 运行中*/
        Stopped,/**< 停止*/
        Deleted/**< 删除*/
    };


public:

    /**
     * @brief 读取插件
     *
     * @fn read
     * @param fileName插件名称
     * @return bool是否读取成功
     */
    bool read(const QString &fileName);

    /**
     * @brief解析依赖项 给定一组插件，判断当前插件的依赖项是否齐全 如果齐全则将
     *
     * @fn resolveDependencies
     * @param specs
     * @return bool
     */
    bool resolveDependencies(const QList<PluginSpec*> &specs);

    /**
     * @brief加载插件
     *
     * @fn loadLibrary
     * @return bool
     */
    bool loadLibrary();

    /**
     * @brief初始化插件
     *
     * @fn initializePlugin
     * @return bool
     */
    bool initializePlugin();

    /**
     * @brief扩展初始化
     *
     * @fn initializeExtensions
     * @return bool
     */
    bool initializeExtensions();

    /**
     * @brief延迟初始化
     *
     * @fn delayedInitialize
     * @return bool
     */
    bool delayedInitialize();
    /**
     * @brief关闭插件
     *
     * @fn kill
     */
    void kill();

    /**
     * @brief给定插件名称和版本，判断是否满足要求
     *
     * @fn provides
     * @param pluginName
     * @param version
     * @return bool
     */
    bool provides(const QString &pluginName,const QString &version) const;

    /**
     * @brief读取json文件中的插件元信息
     *
     * @fn readMetaData
     * @param metaData
     * @return bool
     */
    bool readMetaData(const QJsonObject &metaData);
    /**
     * @brief
     *
     * @fn versionCompare
     * @param version1
     * @param version2
     * @return int
     */
    int versionCompare(const QString &version1, const QString &version2) const; // 比较版本 version1 > version2 返回1，反之返回-1, 相等返回0
private:

    QPluginLoader loader; /**< TODO: describe */

    /////////////////////////////////////////////////////////////////
    /// \brief 下面这些都是从插件元信息中读取
    ///
    QString name; /**< 插件名称 */
    QString version; /**< 版本号 */
//    QString compatVersion;
//    bool required;
    bool experimental; /**< 是否是试验版本 */
//    bool enabledByDefault;
    QString vendor; /**< TODO: describe */
    QString copyright; /**< TODO: describe */
    QString license; /**< TODO: describe */
    QString url; /**< TODO: describe */
    QString category; /**< TODO: describe */
//    QRegExp platformSpecification;
    QVector<PluginDependency> dependencies; /**< TODO: describe */
    /////////////////////////////////////////////////////////////////


    bool enabledBySettings; /**< TODO: describe */
    bool enabledIndirectly; /**< TODO: describe */
    bool forceEnabled; /**< TODO: describe */
    bool forceDisabled; /**< TODO: describe */
    QString location; /**< TODO: describe */
    QString filePath; /**< TODO: describe */
    QStringList arguments; /**< TODO: describe */
    QHash<PluginDependency,PluginSpec*> dependencySpecs; /**< TODO: describe */

    IPlugin* plugin; /**< TODO: describe */
    PluginSpec::State state; /**< TODO: describe */
    bool hasError; /**< TODO: describe */
    QString errorString; /**< TODO: describe */

};
}

#endif // PLUGINSPEC_H
