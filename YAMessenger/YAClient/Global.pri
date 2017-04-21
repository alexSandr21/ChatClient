INCLUDEPATH += $$PWD

DBASELIB = $$PWD/src/DataBaseLib
MODELLIB = $$PWD/src/ModelLib
PRESENTERLIB = $$PWD/src/PresenterLib

release: DESTDIR = $$PWD/bin/release
debug:   DESTDIR = $$PWD/bin/debug

OBJECTS_DIR = $$PWD/obj
LIB_DIR = $$PWD/lib

#MOC_DIR = $$DESTDIR/moc

