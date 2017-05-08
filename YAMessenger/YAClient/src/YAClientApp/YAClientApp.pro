#-------------------------------------------------
#
# Project created by QtCreator 2017-04-09T14:02:11
#
#-------------------------------------------------

include($$PWD/../../Global.pri)

QT += core
QT += qml quick
QT += sql
QT += network
CONFIG += c++11

QML_IMPORT_PATH = ../QML
RESOURCES += ../QML/qml.qrc

TARGET = YAClientApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PRESENTERLIB
INCLUDEPATH += $$MODELLIB
INCLUDEPATH += $$DBASELIB

LIBS += -L$$LIB_DIR

LIBS += \
-lPresenterLib \
-lModelLib \
-lDataBaseLib

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp

HEADERS  +=
