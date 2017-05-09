INCLUDEPATH += $$PWD

DBASELIB = $$PWD/src/DataBaseLib
MODELLIB = $$PWD/src/ModelLib
PRESENTERLIB = $$PWD/src/PresenterLib
PYTHON_EXE = $$PWD/../Externals/Python/python.exe
SRC = $$PWD
EXTERNALS = $$PWD/../Externals

release: DESTDIR = $$PWD/bin/release
debug:   DESTDIR = $$PWD/bin/debug

OBJECTS_DIR = $$PWD/obj
LIB_DIR = $$PWD/lib


