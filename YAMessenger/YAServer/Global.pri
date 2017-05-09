INCLUDEPATH += $$PWD

LIBDIR = $$PWD/lib
DBMANAGER_L = $$PWD/src/DatabaseManagerLib
YASERVER_L = $$PWD/src/YAServerLib
PYTHON_EXE = $$PWD/../Externals/Python/python.exe
SRC = $$PWD
EXTERNALS = $$PWD/../Externals

release: DESTDIR = $$PWD/bin/release
debug:   DESTDIR = $$PWD/bin/debug

OBJECTS_DIR = $$PWD/obj

#QMAKE_POST_LINK += \"$${PYTHON_EXE}\" \"$${SRCROOTDIR}/BuildScripts/copy_files.py\" \"$${SRCROOTDIR}/BuildScripts/buildEPV.ini\" \"$${SRCROOTDIR}/..\"
