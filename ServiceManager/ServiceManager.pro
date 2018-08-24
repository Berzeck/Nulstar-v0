QT -= gui
QT += websockets
CONFIG -= app_bundle
CONFIG += c++11 console
DEFINES += QT_DEPRECATED_WARNINGS
HEADERS += ServiceManager.h

SOURCES += ServiceManager.cpp

TARGET = ServiceManager
DESTDIR = $$DESTDIR/MainComponents
VERSION = 0.0.1.1
DEFINES += APP_NAME=\\\"Nulstar_Service_Manager\\\" APP_VERSION=\\\"$$VERSION\\\" APP_VERSION_NAME=\\\"Baby_Apple_I\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Libraries/Network/release/ -lNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Libraries/Network/debug/ -lNetwork
else:unix: LIBS += -L$$DESTDIR/../Libraries/ -lNetwork

INCLUDEPATH += $$PWD/../Libraries/Network
DEPENDPATH += $$PWD/../Libraries/Network
QMAKE_RPATHDIR = $$DESTDIR/../Libraries/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
