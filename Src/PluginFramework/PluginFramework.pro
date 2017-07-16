TEMPLATE = subdirs

SUBDIRS += \
    app \
    Lib \
    Plugins \
    Test


app.depends = Lib
Plugins.depends = Lib


include(.\PluginFramework_variable.pri)
include(.\PluginFramework_function.pri)
