#include <QString>
#include <QtTest>

#define private public
#include "iplugin.h"
#include "pluginspec.h"
#undef private

class ExtensionSystemTestTest : public QObject
{
    Q_OBJECT

public:
    ExtensionSystemTestTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();

    void test_Initialize();
    void test_addObjectToPool();
    void test_addAutoReleaseObjectToPool();
    void test_removeObjectFromPool();


    //////////////////////////////////////////////////////
    // 测试pluginSpe
    // 版本比较
    void testPluginSpec_VersionCompare_data();
    void testPluginSpec_VersionCompare();

    // 依赖项
    void testPluginSpec_ResolveDepencies1();
    void testPluginSpec_ResolveDepencies2();

};

ExtensionSystemTestTest::ExtensionSystemTestTest()
{
}

void ExtensionSystemTestTest::initTestCase()
{
}

void ExtensionSystemTestTest::cleanupTestCase()
{
}

void ExtensionSystemTestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}


void ExtensionSystemTestTest::test_Initialize()
{
    ExtensionSystem::IPlugin p;

    QStringList arguments;
    QString error;
    QVERIFY(p.initialize(arguments,error));
}

void ExtensionSystemTestTest::test_addObjectToPool()
{
//    ExtensionSystem::IPlugin p;

//    QStringList arguments;
//    QString error;
//    QObject *obj = new QObject();
//    obj->setObjectName(addObj);
//    QVERIFY(p.addObject(obj));
}

void ExtensionSystemTestTest::test_addAutoReleaseObjectToPool()
{
    ExtensionSystem::IPlugin p;

    QStringList arguments;
    QString error;
    {
        QObject obj;
        obj.setObjectName("autoReleaseObj");
        QVERIFY(p.addAutoReleasedObject(&obj));
    }
//    QVERIFY(p.get)
}

void ExtensionSystemTestTest::test_removeObjectFromPool()
{

}

void ExtensionSystemTestTest::testPluginSpec_VersionCompare_data()
{
    QTest::addColumn<QString>("version1");
    QTest::addColumn<QString>("version2");
    QTest::addColumn<int>("compareResult");

    QTest::newRow("v1 larger than v2 in major")<<QString::fromLatin1("4.2.2")<<QString::fromLatin1("3.2.0")<<1;
    QTest::newRow("v1 larger than v2 in minor")<<QString::fromLatin1("4.2.2")<<QString::fromLatin1("4.1.0")<<1;
    QTest::newRow("v1 larger than v2 in patch")<<QString::fromLatin1("4.2.2")<<QString::fromLatin1("4.2.1")<<1;

    QTest::newRow("v1 less than v2 in major")<<QString::fromLatin1("3.2.0")<<QString::fromLatin1("4.2.2")<<-1;
    QTest::newRow("v1 less than v2 in minor")<<QString::fromLatin1("4.1.0")<<QString::fromLatin1("4.2.2")<<-1;
    QTest::newRow("v1 less than v2 in patch")<<QString::fromLatin1("4.2.1")<<QString::fromLatin1("4.2.2")<<-1;

    QTest::newRow("equal")<<QString::fromLatin1("4.2.2")<<QString::fromLatin1("4.2.2")<<0;
    QTest::newRow("invalid")<<QString::fromLatin1("zly")<<QString::fromLatin1("hy")<<0;

}

void ExtensionSystemTestTest::testPluginSpec_VersionCompare()
{
    QFETCH(QString,version1);
    QFETCH(QString,version2);
    QFETCH(int,compareResult);

    ExtensionSystem::PluginSpec pluginSpec;

    QCOMPARE(pluginSpec.versionCompare(version1,version2),compareResult);
}

void ExtensionSystemTestTest::testPluginSpec_ResolveDepencies1()
{
    ExtensionSystem::PluginSpec pluginSepc;

    // 第一个依赖项
    ExtensionSystem::PluginDependency dep;
    dep.name = "core";
    dep.version = "1.2.0";
    dep.type = ExtensionSystem::PluginDependency::Required;

    pluginSepc.dependencies.append(dep);

    // 第二个依赖项
    dep.name = "test";
    dep.version = "1.3.0";
    dep.type = ExtensionSystem::PluginDependency::Required;

    QList<ExtensionSystem::PluginSpec*> pluginSepcList;
    ExtensionSystem::PluginSpec* spec = new ExtensionSystem::PluginSpec();
    spec->name = "core";
    spec->version = "1.2.0";
    pluginSepcList.append(spec);
    spec = new ExtensionSystem::PluginSpec();
    spec->name = "test";
    spec->version = "1.3.0";
    pluginSepcList.append(spec);

    QVERIFY(pluginSepc.resolveDependencies(pluginSepcList));

}

void ExtensionSystemTestTest::testPluginSpec_ResolveDepencies2()
{
    ExtensionSystem::PluginSpec pluginSepc;

    // 第一个依赖项
    ExtensionSystem::PluginDependency dep;
    dep.name = "core";
    dep.version = "1.2.0";
    dep.type = ExtensionSystem::PluginDependency::Required;

    pluginSepc.dependencies.append(dep);

    // 第二个依赖项
    dep.name = "test";
    dep.version = "1.3.0";
    dep.type = ExtensionSystem::PluginDependency::Required;

    QList<ExtensionSystem::PluginSpec*> pluginSepcList;
    ExtensionSystem::PluginSpec* spec = new ExtensionSystem::PluginSpec();
    spec->name = "core";
    spec->version = "1.1.0";
    pluginSepcList.append(spec);
    spec = new ExtensionSystem::PluginSpec();
    spec->name = "test";
    spec->version = "1.3.0";
    pluginSepcList.append(spec);

    QVERIFY(!pluginSepc.resolveDependencies(pluginSepcList));
}


QTEST_APPLESS_MAIN(ExtensionSystemTestTest)

#include "tst_extensionsystemtesttest.moc"

