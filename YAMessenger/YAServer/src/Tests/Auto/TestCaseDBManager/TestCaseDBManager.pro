include (../gtestDependency.pri)

QT += sql network
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread


INCLUDEPATH += $$DBMANAGER_L

LIBS += -L$$LIBDIR

LIBS += \
-lDatabaseManagerLib

SOURCES += \
    main.cpp

HEADERS += \
    testdbmanager.h \
    stdafx.h
