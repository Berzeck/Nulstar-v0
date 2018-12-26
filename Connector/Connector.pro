# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = Connector
VERSION = 0.1.0
APP_DOMAIN=Nulstar
APP_ROLE=Role_ConnectionManager
APP_ROLE_VERSION=0.1
APP_NAME=Nulstar Connection Manager
APP_VERSION=$$VERSION
APP_VERSION_NAME=Baby Piglet I

DESTDIR=$$MODULES_OUTDIR/$$APP_DOMAIN/$$TARGET/$$VERSION

# Source Files

QMAKE_SUBSTITUTES += $$TOP_SRCDIR/AppVersion.h.in

HEADERS += \
    NConnectionController.h

SOURCES += Connector.cpp \
    NConnectionController.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork $$LIBRARIES_PATH/NCore
LIBS += -lNNetwork -lNCore

