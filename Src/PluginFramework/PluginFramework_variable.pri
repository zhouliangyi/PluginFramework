#该文件用于设置整个工程的一些全局变量
#当前文件只允许被包含一次，不可循环包含
!isEmpty(PluginFramework_VARIABLE_INCLUDED):error("PluginFramework_variable.pri already included $$_PRO_FILE_ cannot include this file")
PluginFramework_VARIABLE_INCLUDED = 1


#整个框架的版本号
PLUGIN_FRAMEWORK_VERSION=0.1.0
VERSION=$$PLUGIN_FRAMEWORK_VERSION



#当前工程所在目录
SOURCE_TREE = $$PWD
#设置编译的路径 如果人工设置，则采用人工设置的；反之采用输出目录
isEmpty(BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    BUILD_TREE = $$clean_path($$OUT_PWD)
    BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
message($$_PRO_FILE_PWD_ BUILD_TREE $$OUT_PWD)

#APP的路径、名称
APP_PATH = $$BUILD_TREE/bin
APP_TARGET = app

#设置输出路径、库路径、插件路径、数据路径、可执行程序路径
OUTPUT_PATH=$$BUILD_TREE
LIBRARY_PATH=$$OUTPUT_PATH/Lib/libs
PLUGIN_PATH=$$OUTPUT_PATH/Lib/plugins
DATA_PATH=$$OUTPUT_PATH/share
BIN_PATH=$$OUTPUT_PATH/bin
