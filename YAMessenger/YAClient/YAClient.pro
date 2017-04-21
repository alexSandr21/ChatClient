TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
          src/DataBaseLib\
          src/ModelLib\
          src/PresenterLib\
          src/PresenterApp


DataBaseLib.file = DataBaseLib/DataBaseLib.pro
ModelLib.file = ModelLib/ModelLib.pro
PresenterLib.file = PresenterLib/PresenterLib.pro
PresenterApp.file = PresenterApp/PresenterApp.pro

PresenterLib.depends = DataBaseLib ModelLib
PresenterApp.depends = DataBaseLib ModelLib PresenterLib

