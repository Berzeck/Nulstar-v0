# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = Connector
VERSION = 0.0.1.1
DEFINES += APP_DOMAIN=\\\"nulstar.com\\\" APP_ROLE=\\\"Role_ConnectionManager\\\" APP_ROLE_VERSION=\\\"0.1.0\\\" APP_NAME=\\\"Nulstar_Connector\\\" APP_VERSION=\\\"$$VERSION\\\" APP_VERSION_NAME=\\\"Baby_Piglet_I\\\"

DESTDIR=$$MODULES_OUTDIR/$$TARGET/$$VERSION
LIBS += -L$$LIBRARIES_OUTDIR/NNetwork/0.0.1.1/
LIBS += -L$$LIBRARIES_OUTDIR/NCore/0.0.1.1/

# Source Files

HEADERS += \
    NConnectionController.h

SOURCES += Connector.cpp \
    NConnectionController.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork $$LIBRARIES_PATH/NCore
LIBS += -lNNetwork -lNCore

