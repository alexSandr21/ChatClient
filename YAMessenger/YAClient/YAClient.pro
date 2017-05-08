TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
          src/DataBaseLib\
          src/ModelLib\
          src/PresenterLib\
          src/YAClientApp


DataBaseLib.file = DataBaseLib/DataBaseLib.pro
ModelLib.file = ModelLib/ModelLib.pro
PresenterLib.file = PresenterLib/PresenterLib.pro
YAClientApp.file = YAClientApp/YAClientApp.pro

PresenterLib.depends = DataBaseLib ModelLib
YAClientApp.depends = DataBaseLib ModelLib PresenterLib

