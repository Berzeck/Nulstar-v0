# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = ServiceManager
VERSION = 0.1.0
APP_DOMAIN=Nulstar
APP_ROLE=Role_ServiceManager
APP_ROLE_VERSION=0.1
APP_NAME=Nulstar Service Manager
APP_VERSION=$$VERSION
APP_VERSION_NAME=Baby Apple I

DESTDIR=$$MODULES_OUTDIR/$$APP_DOMAIN/$$TARGET/$$VERSION

# Source Files

QMAKE_SUBSTITUTES += $$TOP_SRCDIR/AppVersion.h.in

HEADERS += NServiceManagerController.h

SOURCES += NServiceManagerController.cpp \
           ServiceManager.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork $$LIBRARIES_PATH/NCore
LIBS += -lNNetwork -lNCore
