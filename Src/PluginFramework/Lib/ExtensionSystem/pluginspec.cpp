#include "pluginspec.h"
#include "iplugin.h"

#include <QFileInfo>
#include <QJsonArray>

namespace ExtensionSystem {
namespace CONSTS
{
const char PLUGIN_METADATA[] = "MetaData";
const char PLUGIN_NAME[] = "Name";
const char PLUGIN_VERSION[] = "Version";
const char PLUGIN_COMPATVERSION[] = "CompatVersion";
const char PLUGIN_REQUIRED[] = "Required";
const char PLUGIN_EXPERIMENTAL[] = "Experimental";
const char PLUGIN_DISABLED_BY_DEFAULT[] = "DisabledByDefault";
const char VENDOR[] = "Vendor";
const char COPYRIGHT[] = "Copyright";
const char LICENSE[] = "License";
const char DESCRIPTION[] = "Description";
const char URL[] = "Url";
const char CATEGORY[] = "Category";
const char PLATFORM[] = "Platform";
const char DEPENDENCIES[] = "Dependencies";
const char DEPENDENCY_NAME[] = "Name";
const char DEPENDENCY_VERSION[] = "Version";
const char DEPENDENCY_TYPE[] = "Type";
const char DEPENDENCY_TYPE_SOFT[] = "optional";
const char DEPENDENCY_TYPE_HARD[] = "required";
const char DEPENDENCY_TYPE_TEST[] = "test";
const char ARGUMENTS[] = "Arguments";
const char ARGUMENT_NAME[] = "Name";
const char ARGUMENT_PARAMETER[] = "Parameter";
const char ARGUMENT_DESCRIPTION[] = "Description";

}

bool PluginDependency::operator==(const PluginDependency &other) const
{
    return name == other.name && version == other.version && type == other.type;
}

//static QString typeString(PluginDependency::Type type)
//{
//    switch (type) {
//    case PluginDependency::Optional:
//        return QString(", optional");
//    case PluginDependency::Test:
//        return QString(", test");
//    case PluginDependency::Required:
//    default:
//        return QString();
//    }
//}

//QString PluginDependency::toString() const
//{
//    return name + " (" + version + typeString(type) + ")";
//}

uint qHash(const PluginDependency &value)
{
    return qHash(value.name);
}

}

ExtensionSystem::PluginSpec::PluginSpec()
{

}

bool ExtensionSystem::PluginSpec::read(const QString &fileName)
{
    // 读取的初始化操作
    name
            = version
//            = compatVersion
            = vendor
            = copyright
            = license
            = url
            = category
            = location
            = QString();

    state = Invalid;
    hasError = false;
    errorString.clear();
    dependencies.clear();

    // 开始读取
    QFileInfo fileInfo(fileName);
    location = fileInfo.absolutePath();
    filePath = fileInfo.absoluteFilePath();
    loader.setFileName(filePath);
    // 如果加载器对应的名字为空，则读取失败
    if (loader.fileName().isEmpty()) {
        return false;
    }
    if (!readMetaData(loader.metaData())) {
        return false;
    }

    state = Read;
    return true;
}

bool ExtensionSystem::PluginSpec::resolveDependencies(const QList<ExtensionSystem::PluginSpec *> &specs)
{
    QHash<PluginDependency, PluginSpec *> resolvedDependencies;
    // 遍历该插件所依赖的所有插件
    foreach (const PluginDependency &dep, dependencies)
    {
        bool findSpec = false;
        foreach (PluginSpec * const spec, specs)
        {
            // 寻找满足依赖条件的插件
            if(spec->provides(dep.name,dep.version))
            {
                // 将该插件的信息存入插件中
                resolvedDependencies.insert(dep,spec);
                findSpec = true;
            }
        }
        // 如果找不到依赖的插件 则解析失败
        if(!findSpec)
            return false;
    }
    dependencySpecs = resolvedDependencies;
    state = PluginSpec::Resolved;
    return true;
}

bool ExtensionSystem::PluginSpec::loadLibrary()
{
    // 如果插件加载有错则返回
   if(hasError)
       return false;
   // 判断当前插件状态是否是已解析
   if(state != Resolved)
   {
       // 如果当前已经是加载状态 则返回正确
       if(state == Loaded)
       {
           return true;
       }
       // 如果当前不是加载状态，则返回失败
       else
       {
           hasError = true;
           return false;
       }
   }
   // 判断插件加载是否成功
   if(!loader.load())
   {
       hasError = true;
       errorString = loader.errorString();
       return false;
   }
   // 获取插件中的对象 并判断是否有内容
   IPlugin* pluginObj = qobject_cast<IPlugin*>(loader.instance());
   if(!pluginObj)
   {
       loader.unload();
       return false;
   }
   // 加载成功后将插件状态设置为已加载，成员变量赋值
   state = Loaded;
   plugin=pluginObj;
   plugin->setPluginSpec(this);
   return true;
}

bool ExtensionSystem::PluginSpec::initializePlugin()
{
    // 如果错误 则返回false
    if (hasError) {
        return false;
    }
    // 如果状态不是已加载则返回false
    if (state != Loaded) {
        if(state != Initialized)
        {
            hasError = true;
            return false;
        }
        else
        {
            return true;
        }
    }
    // 如果对应指针为空 则返回false
    if(!plugin)
    {
        hasError = true;
        return false;
    }
    // 如果初始化失败 则返回false
    if(!plugin->initialize(arguments,errorString))
    {
        hasError = true;
        return false;
    }
    // 初始化成功 设置插件当前状态，返回true
    state = Initialized;
    return true;
}

bool ExtensionSystem::PluginSpec::initializeExtensions()
{
    // 如果错误 则返回
    if(hasError){
        return false;
    }
    // 如果当前状态不是初始化状态，则返回
    if(this->state != Initialized){
        hasError = true;
        return false;
    }
    // 如果插件的对象为空 则返回false
    if(!plugin){
        hasError=true;
        return false;
    }
    // 扩展初始化失败
    if(!plugin->initializeExtensions(this->arguments,this->errorString)){
        hasError = true;
        return false;
    }
    // 初始化成功
    this->state = Running;
    return true;
}

bool ExtensionSystem::PluginSpec::provides(const QString &pluginName, const QString &version) const
{
    // 如果输入名称和当前插件名称不相同，返回false
    if(QString::compare(pluginName,this->name,Qt::CaseInsensitive) != 0)
        return false;
    if (this->versionCompare(this->version,version) >= 0)
        return true;
    return false;
}

bool ExtensionSystem::PluginSpec::readMetaData(const QJsonObject &metaData)
{
    QJsonValue value;
    // 判断IID是否与pluginManager中一致 如果不一致则初始化失败
    value = metaData.value(QLatin1String("IID"));
    if(!value.isString())
    {
        return false;
    }
    // TODO 判断与pluginmanager是否一致
//    if(value.toString() != )

    // 如果插件元信息中不含metaInfo 则直接返回错误
    value = metaData.value(QLatin1String(CONSTS::PLUGIN_METADATA));
    if(!value.isObject())
        return false;
    QJsonObject pluginInfo = value.toObject();

    // 插件名称
    value = pluginInfo.value(QLatin1String(CONSTS::PLUGIN_NAME));
    if (value.isUndefined() || !value.isString()) {
        return false;
    }
    name = value.toString();

    // 版本号
    value = pluginInfo.value(QLatin1String(CONSTS::PLUGIN_VERSION));
    if (value.isUndefined() || !value.isString()) {
        return false;
    }
    version = value.toString();

//    // 兼容版本
//    value = pluginInfo.value(CONSTS::PLUGIN_COMPATVERSION);
//    if (value.isUndefined() || !value.isString()) {
//        return false;
//    }
//    compatVersion = value.toString();

    // 试验版本
    value = pluginInfo.value(CONSTS::PLUGIN_EXPERIMENTAL);
    if (!value.isUndefined() && !value.isBool())
    {
        return false;
    }
    experimental = value.toBool(false);


    // 提供者
    value = pluginInfo.value(CONSTS::VENDOR);
    if (!value.isUndefined() && !value.isString()) {
        return false;
    }
    vendor = value.toString();

    // 版权
    value = pluginInfo.value(CONSTS::COPYRIGHT);
    if (!value.isUndefined() && !value.isString()) {
        return false;
    }
    copyright = value.toString();

    // 许可
    value = pluginInfo.value(CONSTS::LICENSE);
    if (!value.isUndefined() && !value.isArray()) {
        return false;
    }
    QJsonArray licenseArray = value.toArray();
    foreach (const QJsonValue v, licenseArray) {
        if(!v.isString())
            return false;
        license += v.toString();
    }
//    license = value.toString();

    // 链接
    value = pluginInfo.value(CONSTS::URL);
    if (!value.isUndefined() && !value.isString()) {
        return false;
    }
    url = value.toString();

    // 归类
    value = pluginInfo.value(CONSTS::CATEGORY);
    if (!value.isUndefined() && !value.isString()) {
        return false;
    }
    category = value.toString();

    // 依赖项
    value = pluginInfo.value(QLatin1String(CONSTS::DEPENDENCIES));
    if (!value.isUndefined() && !value.isArray()) {
        return false;
    }
    // 依赖项是一个序列
    QJsonArray array = value.toArray();
    foreach (const QJsonValue v, array) {
       // 获取每个依赖项
        if(!v.isObject()){
            return false;
        }
        QJsonObject depencyObject = v.toObject();
        PluginDependency dep;
        // 依赖项名称
        value = depencyObject.value(QLatin1String(CONSTS::DEPENDENCY_NAME));
        if (!value.isUndefined() && !value.isString()) {
            return false;
        }
        dep.name = value.toString();
        // 依赖项版本
        value = depencyObject.value(QLatin1String(CONSTS::DEPENDENCY_VERSION));
        if (!value.isUndefined() && !value.isString()) {
            return false;
        }
        dep.version = value.toString();
        // 依赖项类型 默认为需要
        dep.type = PluginDependency::Required;
        value = depencyObject.value(QLatin1String(CONSTS::DEPENDENCY_TYPE));
        if (!value.isUndefined() && !value.isString()) {
            return false;
        }
        QString typeValue = value.toString();
        if (typeValue.toLower() == QLatin1String(CONSTS::DEPENDENCY_TYPE_HARD)) {
            dep.type = PluginDependency::Required;
        } else if (typeValue.toLower() == QLatin1String(CONSTS::DEPENDENCY_TYPE_SOFT)) {
            dep.type = PluginDependency::Optional;
        } else if (typeValue.toLower() == QLatin1String(CONSTS::DEPENDENCY_TYPE_TEST)) {
            dep.type = PluginDependency::Test;
        } else {
            dep.type = PluginDependency::Required;
        }

        this->dependencies.append(dep);
    }
}

int ExtensionSystem::PluginSpec::versionCompare(const QString &version1, const QString &version2) const
{
    QRegExp reg1(QLatin1String("([0-9]+)(?:[.]([0-9]+))?(?:[.]([0-9]+))?(?:_([0-9]+))?"));
    QRegExp reg2(QLatin1String("([0-9]+)(?:[.]([0-9]+))?(?:[.]([0-9]+))?(?:_([0-9]+))?"));

    if (!reg1.exactMatch(version1))
        return 0;
    if(!reg2.exactMatch(version2))
        return 0;

    int number1;
    int number2;
    for (int i = 0; i < 4; ++i) {
        number1 = reg1.cap(i+1).toInt();
        number2 = reg2.cap(i+1).toInt();
        if (number1 < number2) {
            return -1;
        }
        if (number1 > number2) {
            return 1;
        }
    }
    return 0;
}


















