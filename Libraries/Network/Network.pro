## [DEFAULT]
QT -= gui
QT += websockets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += Network.h \
           NNulstarWebSocketsServer.h

SOURCES += NNulstarWebSocketsServer.cpp

unix {
  target.path = /usr/lib
  INSTALLS += target
}

## [CUSTOM]
TEMPLATE = lib
TARGET = Network
DESTDIR = $$DESTDIR/Libraries
VERSION = 0.0.1.1
DEFINES += NETWORK_LIBRARY

