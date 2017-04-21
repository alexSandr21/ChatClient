INCLUDEPATH += $$PWD

LIBDIR = $$PWD/lib
DBMANAGER_L = $$PWD/src/DatabaseManagerLib
YASERVER_L = $$PWD/src/YAServerLib

release: DESTDIR = $$PWD/bin/release
debug:   DESTDIR = $$PWD/bin/debug

OBJECTS_DIR = $$PWD/obj
