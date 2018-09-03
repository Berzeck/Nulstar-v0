QT -= gui
QT += websockets
CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS
HEADERS += NMainController.h
SOURCES += NMainController.cpp \
           NulstarMain.cpp

TARGET = Nulstar
VERSION = 0.0.1.1
DEFINES += APP_NAME=\\\"Nulstar_Main_Controller\\\" APP_VERSION=\\\"$$VERSION\\\" APP_VERSION_NAME=\\\"OneEye_Baby_Ninja_Egg\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/Libraries/Network/release/ -lNetwork -L$$OUT_PWD/Libraries/Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/Libraries/Network/debug/ -lNetwork -L$$OUT_PWD/Libraries/Core/debug/ -lCore
else:unix: LIBS += -L$$DESTDIR/Libraries/ -lNetwork -L$$DESTDIR/Libraries/ -lCore

QMAKE_LFLAGS_RELEASE += -s
INCLUDEPATH += $$PWD/../Libraries/Network $$PWD/../Libraries/Core
DEPENDPATH += $$PWD/../Libraries/Network $$PWD/../Libraries/Core
QMAKE_RPATHDIR = $$DESTDIR/Libraries/

config_files.files = $$PWD/Nulstar.cfg $$PWD/Constants.ncf
config_files.path = $$DESTDIR
INSTALLS += config_files

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
