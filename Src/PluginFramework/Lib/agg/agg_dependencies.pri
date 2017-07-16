#functionFile = ..\..\PluginFramework_function.pri


#exists($$functionFile){
#    include($$functionFile)
#}else{
#    error(no file)
#}

# 获得当前pro文件的名称
#LIB_NAME = $$relative_path($$_PRO_FILE_,$$_PRO_FILE_PWD_)
#LIB_NAME = $$split(($$LIB_NAME),.)
LIB_NAME=agg
LIB_DEPENDS =
