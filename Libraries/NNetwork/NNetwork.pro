# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets

CONFIG += c++11 skip_target_version_ext

TEMPLATE = lib
TARGET = NNetwork
VERSION = 0.0.1.1
DEFINES += NETWORK_LIBRARY

DESTDIR=$$LIBRARIES_OUTDIR

# Source Files

HEADERS += NNetwork.h \
           NResponse.h \
           NRequest.h \
           NWebSocketServer.h \
    NAck.h \
    NUnsubscribe.h \
    NMessage.h \
    NMessageNegotiateConnection.h \
    NMessageNegotiateConnectionResponse.h \
    NMessageUnregisterCompoundMethod.h  \
    NNotification.h \
    NRegisterCompoundMethod.h \
    NUnregisterCompoundMethod.h \
    NMessageRegisterCompoundMethod.h

SOURCES += NResponse.cpp \
           NRequest.cpp \
           NWebSocketServer.cpp \
    NAck.cpp \
    NUnsubscribe.cpp \
    NMessage.cpp \
    NMessageNegotiateConnection.cpp \
    NMessageNegotiateConnectionResponse.cpp \
    NMessageUnregisterCompoundMethod.cpp  \
    NNotification.cpp \
    NRegisterCompoundMethod.cpp \
    NUnregisterCompoundMethod.cpp \
    NMessageRegisterCompoundMethod.cpp

# Dependencies

