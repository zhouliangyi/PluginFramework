#该文件用于设置lib，包括lib对应的依赖项、引用qtcreator.pri中的全局变量、设置lib的安装位置、名称等等
dependPriFileName=$$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
exists($$dependPriFileName){
    include($$dependPriFileName)
}else{
error(cannot included $$dependPriFileName in $$_PRO_FILE_PWD_)
}
include(../PluginFramework_variable.pri)
include(../PluginFramework_function.pri)

TARGET = $$LibraryTargetName($$LIB_NAME)
DLLDESTDIR = $$BIN_PATH
DESTDIR = $$LIBRARY_PATH
TEMPLATE = lib
CONFIG+= shared dll

#依赖关系变量在dependencies.pri文件中定义的
LIB_DEPENDS = $$parseDepends($$LIB_DEPENDS)
#for(libName,LIB_DEPENDS){
#    LIBS += -l$$libName
#}

#LIBS += -L$$LIBRARY_PATH
