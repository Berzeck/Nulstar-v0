# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings

QT -= gui
QT += network websockets
CONFIG += c++11

TEMPLATE = lib
TARGET = NCore
VERSION = 0.0.1.1
DEFINES += CORE_LIBRARY

DESTDIR=$$LIBRARIES_OUTDIR

# Source Files

HEADERS += Core.h \
           NCoreService.h \
    NApiBuilder.h \
    NPacketProcessor.h

SOURCES += NCoreService.cpp \
    NApiBuilder.cpp \
    NPacketProcessor.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork
LIBS += -lNNetwork
