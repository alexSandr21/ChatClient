#-------------------------------------------------
#
# Project created by QtCreator 2017-04-09T12:40:36
#
#-------------------------------------------------

include($$PWD/../../Global.pri)



QT += qml sql
QT       -= gui

TARGET = PresenterLib
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$DBASELIB
INCLUDEPATH += $$MODELLIB

LIBS += -L$$LIB_DIR -lDataBaseLib -lModelLib

PRECOMPILED_HEADER = stdafx.h

SOURCES += presenter.cpp \
    filesendthread.cpp

HEADERS += presenter.h \
    stdafx.h \
    filesendthread.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$LIB_DIR
DISTFILES +=
message($$LIB_DIR)
