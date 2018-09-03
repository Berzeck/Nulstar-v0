QT -= gui
QT += websockets
CONFIG -= app_bundle
CONFIG += c++11 console
DEFINES += QT_DEPRECATED_WARNINGS
HEADERS += \
    NConnectionController.h

SOURCES += Connector.cpp \
    NConnectionController.cpp

TARGET = Connector
DESTDIR = $$DESTDIR/MainComponents
VERSION = 0.0.1.1
DEFINES += APP_NAME=\\\"Nulstar_Connector\\\" APP_VERSION=\\\"$$VERSION\\\" APP_VERSION_NAME=\\\"Baby_Piglet_I\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Libraries/Network/release/ -lNetwork -L$$OUT_PWD/../Libraries/Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Libraries/Network/debug/ -lNetwork -L$$OUT_PWD/../Libraries/Core/debug/ -lCore
else:unix: LIBS += -L$$DESTDIR/../Libraries/ -lNetwork -L$$DESTDIR/../Libraries/ -lCore

QMAKE_LFLAGS_RELEASE += -s
INCLUDEPATH += $$PWD/../Libraries/Network $$PWD/../Libraries/Core
DEPENDPATH += $$PWD/../Libraries/Network $$PWD/../Libraries/Core
QMAKE_RPATHDIR = $$DESTDIR/../Libraries/

#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
