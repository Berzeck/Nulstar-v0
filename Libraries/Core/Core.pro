## [DEFAULT]
QT -= gui
QT += network websockets
CONFIG += c++11 skip_target_version_ext

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += Core.h \
           NCoreService.h \
    NApiBuilder.h \
    NPacketProcessor.h

SOURCES += NCoreService.cpp \
    NApiBuilder.cpp \
    NPacketProcessor.cpp

LIBS += -L$$DESTDIR/Libraries/ -lNetwork

INCLUDEPATH += $$PWD/../Network
DEPENDPATH += $$PWD/../Network
unix:QMAKE_LFLAGS_RELEASE += -s
TEMPLATE = lib
TARGET = Core
DESTDIR = $$DESTDIR/Libraries
VERSION = 0.0.1.1
DEFINES += CORE_LIBRARY

