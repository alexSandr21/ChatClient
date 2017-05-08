TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
          src/DatabaseManagerLib\
          src/YAServerLib\
          src/YAServerApp

PYTHON = $$PWD/../Externals/Python/python.exe

win32{
    CONFIG(debug, debug|release) {
        system($$PYTHON $$PWD/PreBuild.py $$PWD $$PWD/../Externals debug)
    }
    else{
        system($$PYTHON $$PWD/PreBuild.py $$PWD $$PWD/../Externals)
    }
}

CONFIG(debug, debug|release) {
    SUBDIRS += src/Tests
}

YAServerApp.file = YAServerApp/YAServerApp.pro
DatabaseManagerLib.file = DatabaseManagerLib/DatabaseManagerLib.pro
YaServerLib.file = YAServerLib/YaServerLib.pro

YAServerApp.depends = YAServerLib
YAServerLib.depends = DatabaseManagerLib

