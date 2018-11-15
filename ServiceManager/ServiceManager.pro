# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = ServiceManager
VERSION = 0.0.1.1
DEFINES += APP_DOMAIN=\\\"nulstar.com\\\" APP_ROLE=\\\"Role_ServiceManager\\\" APP_ROLE_VERSION=\\\"0.1.0\\\" APP_NAME=\\\"Nulstar_Service_Manager\\\" APP_VERSION=\\\"$$VERSION\\\" APP_VERSION_NAME=\\\"Baby_Apple_I\\\"

DESTDIR=$$MAIN_MODULES_OUTDIR

# Source Files

HEADERS += NServiceManagerController.h

SOURCES += NServiceManagerController.cpp \
           ServiceManager.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork $$LIBRARIES_PATH/NCore
LIBS += -lNNetwork -lNCore
