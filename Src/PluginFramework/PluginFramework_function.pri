#该文件用于设置整个工程的一些全局函数
#通过***_dependices.pri文件获取对应的target名称
!isEmpty(PluginFramework_FUNCTION_INCLUDED):error("PluginFramework_function.pri already included")
PluginFramework_FUNCTION_INCLUDED = 1


defineReplace(dependicesPriFileName) {
   RET = $$1
   message($$RET)
   NAME_LIST = $$split(RET, .)
message($$NAME_LIST)
   RET = $$first(NAME_LIST)
message($$RET)
   return($$RET)
}

#定义qtLibraryTargetName函数 获取qt库目标的名称，根据debug或者release
defineReplace(LibraryTargetName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1

   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}
#将libraryTargetName加上QtCreator的major版本
defineReplace(LibraryName) {
   RET = $$LibraryTargetName($$1)
   win32 {
      VERSION_LIST = $$split(QTCREATOR_VERSION, .)
      RET = $$RET$$first(VERSION_LIST)
   }
   return($$RET)
}


defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}


# For use in custom compilers which just copy files
defineReplace(stripSrcDir) {
    return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
}


#解析库之间的依赖关系 获取当前工程依赖的其他库名称
#输入依赖的库

defineReplace(parseDepends){
#输入的参数定义为LIB_DEPENDS，与后面include引入的变量保持一致
    LIBDEP=$$1
    done_libs =
    for(ever) {
#如果无依赖，则停止
        isEmpty(LIBDEP): break()
#done_libs存放已经解析完成的库
        done_libs += $$LIBDEP
        #将所有依赖的库对应的depencices都包含一遍，解析其中的LIBDEP，迭代解析完所有依赖关系
        for(dep, LIBDEP) {
            include($$PWD/$$dep/$${dep}_dependencies.pri)
            LIBDEP+=$$LIB_DEPENDS
#            LIBS += -l$$qtLibraryName($$QTC_LIB_NAME)
        }
#将重复依赖的库去掉，并且去掉已经解析完的库
        LIBDEP = $$unique(LIBDEP)
        LIBDEP -= $$unique(done_libs)
    }
#message($$_PRO_FILE_ done--$$done_libs)
    return($$done_libs)
}

##链接函数，输入为依赖库的列表，将其链接到工程中
#defineTest(linkDepList){
#    depLibs = $$1
#    for(libName,depLibs){
#        LIBS += -l$$libName
#    }
#}
