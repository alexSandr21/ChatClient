include($$PWD/../../Global.pri)

QT += core network sql gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YAServerApp
TEMPLATE = app
CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

win32{
    CONFIG(debug, debug|release){
        QMAKE_POST_LINK = \"$${PYTHON_EXE}\" \"$${SRC}/PreBuild.py\" \"$${SRC}\" \"$${EXTERNALS}\" \"debug\"
    }
    else{
        QMAKE_POST_LINK = \"$${PYTHON_EXE}\" \"$${SRC}/PreBuild.py\" \"$${SRC}\" \"$${EXTERNALS}\"
    }
}

INCLUDEPATH += \
$$DBMANAGER_L\
$$YASERVER_L

LIBS += -L$$LIBDIR

LIBS += \
-lYAServerLib\
-lDatabaseManagerLib

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += main.cpp

HEADERS += \
    stable.h \
    stdafx.h

