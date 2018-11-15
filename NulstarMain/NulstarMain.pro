# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = Nulstar
VERSION = 0.0.1.1
DEFINES += APP_DOMAIN=\\\"nulstar.com\\\" APP_ROLE=\\\"Role_ServiceController\\\" APP_ROLE_VERSION=\\\"0.1.0\\\" APP_NAME=\\\"Nulstar_Main_Controller\\\" APP_VERSION=\\\"$$VERSION\\\" APP_VERSION_NAME=\\\"OneEye_Baby_Ninja_Egg\\\"


# Source Files

HEADERS += NMainController.h
SOURCES += NMainController.cpp \
           NulstarMain.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork $$LIBRARIES_PATH/NCore
LIBS += -lNNetwork -lNCore

# Install Settings

QMAKE_POST_LINK += $$QMAKE_COPY \"$$PWD/Nulstar.cfg\" \"$$DESTDIR\"
#QMAKE_POST_LINK += $$quote($$QMAKE_COPY \"$$PWD/Nulstar.cfg\" \"$$DESTDIR\")

# Clean Settings
QMAKE_CLEAN += -r $$DESTDIR
