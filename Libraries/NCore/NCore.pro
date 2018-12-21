# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings

QT -= gui
QT += network websockets
CONFIG += c++11 skip_target_version_ext

TEMPLATE = lib
TARGET = NCore
VERSION = 0.0.1.1
DEFINES += CORE_LIBRARY

DESTDIR=$$LIBRARIES_OUTDIR/$$TARGET/$$VERSION
LIBS += -L$$LIBRARIES_OUTDIR/NNetwork/0.0.1.1/

# Source Files

HEADERS += Core.h \
           NCoreService.h \
    NApiBuilder.h \
    NMessageProcessor.h

SOURCES += NCoreService.cpp \
    NApiBuilder.cpp \
    NMessageProcessor.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork
LIBS += -lNNetwork
