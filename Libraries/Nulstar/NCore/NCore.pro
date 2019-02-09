# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings

QT -= gui
QT += network websockets
CONFIG += c++11 skip_target_version_ext

TEMPLATE = lib
TARGET = NCore
VERSION = $$NULSTAR_LIBRARIES_VERSION
DEFINES += CORE_LIBRARY

DESTDIR=$$LIBRARIES_OUTDIR

# Source Files

HEADERS += Core.h \
           NCoreService.h \
    NApiBuilder.h \
    NModuleAPI.h \
    NCoreConstants.h

SOURCES += NCoreService.cpp \
    NApiBuilder.cpp \
    NModuleAPI.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/Nulstar/NNetwork
LIBS += -lNNetwork
