TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
          src/DatabaseManagerLib\
          src/YAServerLib\
          src/YAServerApp \
    src/CryptoLib

YAServerApp.file = YAServerApp/YAServerApp.pro
DatabaseManagerLib.file = DatabaseManagerLib/DatabaseManagerLib.pro
YaServerLib.file = YAServerLib/YaServerLib.pro

YAServerApp.depends = YAServerLib
YAServerLib.depends = DatabaseManagerLib

