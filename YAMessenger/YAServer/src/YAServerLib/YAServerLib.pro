#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T16:00:48
#
#-------------------------------------------------

include($$PWD/../../Global.pri)
DESTDIR = $$LIBDIR

QT       += gui sql network widgets

TARGET = YAServerLib
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$DBMANAGER_L

LIBS += -L$$LIBDIR

LIBS += \
-lDatabaseManagerLib

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
PRECOMPILED_HEADER = stdafx.h

SOURCES += \
    serverdialog.cpp \
    clientconnection.cpp \
    server.cpp

HEADERS += \
    stdafx.h \
    serverdialog.h \
    clientconnection.h \
    server.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    serverdialog.ui

DISTFILES +=
