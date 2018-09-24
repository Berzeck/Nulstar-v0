QT -= gui
QT += websockets
CONFIG -= app_bundle
CONFIG += c++11 console
DEFINES += QT_DEPRECATED_WARNINGS
HEADERS += NServiceManagerController.h

SOURCES += NServiceManagerController.cpp \
           ServiceManager.cpp

TARGET = ServiceManager
DESTDIR = $$DESTDIR/MainComponents
VERSION = 0.0.1.1
DEFINES += APP_DOMAIN=\\\"nulstar.com\\\" APP_ROLE=\\\"Role_ServiceManager\\\" APP_ROLE_VERSION=\\\"0.1.0\\\" APP_NAME=\\\"Nulstar_Service_Manager\\\" APP_VERSION=\\\"$$VERSION\\\" APP_VERSION_NAME=\\\"Baby_Apple_I\\\"

LIBS += -L$$DESTDIR/../Libraries/ -lNetwork -L$$DESTDIR/../Libraries/ -lCore

unix:QMAKE_LFLAGS_RELEASE += -s
INCLUDEPATH += $$PWD/../Libraries/Network $$PWD/../Libraries/Core
DEPENDPATH += $$PWD/../Libraries/Network $$PWD/../Libraries/Core
QMAKE_RPATHDIR = $$DESTDIR/../Libraries/

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
