# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = Nulstar
VERSION = 0.0.1.1
APP_DOMAIN=nulstar.com
APP_ROLE=Role ServiceController
APP_ROLE_VERSION=0.1.0
APP_NAME=Nulstar Main Controller
APP_VERSION=$$VERSION
APP_VERSION_NAME=OneEye Baby Ninja Egg


# Source Files

QMAKE_SUBSTITUTES += AppVersion.h.in

HEADERS += NMainController.h \
           NPluginManger.h \
           NModulesManager.h \
           NModuleInfo.h

SOURCES += NMainController.cpp \
           NulstarMain.cpp \
           NPluginManger.cpp \
           NModulesManager.cpp \
           NModuleInfo.cpp

OTHER_FILES += Nulstar.cfg \
            Constants.ncf \
            AppVersion.h.in

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork $$LIBRARIES_PATH/NCore
LIBS += -lNNetwork -lNCore

# Install Settings

QMAKE_POST_LINK += $$quote($$QMAKE_COPY \"$$PWD/Nulstar.cfg\" \"$$DESTDIR\" $$escape_expand(\n\t))
QMAKE_POST_LINK += $$quote($$QMAKE_COPY \"$$PWD/Constants.ncf\" \"$$DESTDIR\")

# Clean Settings
QMAKE_CLEAN += -r $$DESTDIR
