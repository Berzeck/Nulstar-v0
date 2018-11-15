# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets

CONFIG += c++11

TEMPLATE = lib
TARGET = NNetwork
VERSION = 0.0.1.1
DEFINES += NETWORK_LIBRARY

DESTDIR=$$LIBRARIES_OUTDIR

# Source Files

HEADERS += Network.h \
           NResponse.h \
           NRequest.h \
           NWebSocketServer.h

SOURCES += NResponse.cpp \
           NRequest.cpp \
           NWebSocketServer.cpp


# Dependencies

