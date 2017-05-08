TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
          src/DataBaseLib\
          src/ModelLib\
          src/PresenterLib\
          src/YAClientApp

PYTHON = $$PWD/../Externals/Python/python.exe

win32{
    CONFIG(debug, debug|release) {
        system($$PYTHON $$PWD/PreBuild.py $$PWD $$PWD/../Externals debug)
    }
    else{
        system($$PYTHON $$PWD/PreBuild.py $$PWD $$PWD/../Externals)
    }
}

DataBaseLib.file = DataBaseLib/DataBaseLib.pro
ModelLib.file = ModelLib/ModelLib.pro
PresenterLib.file = PresenterLib/PresenterLib.pro
YAClientApp.file = YAClientApp/YAClientApp.pro

PresenterLib.depends = DataBaseLib ModelLib
YAClientApp.depends = DataBaseLib ModelLib PresenterLib

