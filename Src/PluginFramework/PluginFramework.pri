#该文件用于设置整个工程的一些全局变量

#当前工程所在目录
IDE_SOURCE_TREE = $$PWD
#设置编译的路径 如果人工设置，则采用人工设置的；反之采用输出目录
isEmpty(IDE_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_BUILD_TREE = $$clean_path($$OUT_PWD)
    IDE_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}

APP_PATH = $$IDE_BUILD_TREE/bin
