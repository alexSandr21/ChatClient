#-------------------------------------------------
#
# Project created by QtCreator 2017-04-09T12:29:16
#
#-------------------------------------------------

include($$PWD/../../Global.pri)

QT       += network

QT       -= gui



TARGET = ModelLib
TEMPLATE = lib
CONFIG += staticlib
CONFIG += precompile_header

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/../Certificates

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = stable.h

SOURCES += model.cpp

HEADERS += model.h \
    clientinfo.h \
    stdafx.h \
    stable.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$LIB_DIR
DISTFILES +=
