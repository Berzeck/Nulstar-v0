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

DESTDIR=$$LIBRARIES_OUTDIR/$$TARGET/$$VERSION

# Source Files

HEADERS += NNetwork.h \
           NMessageResponse.h \
           NMessageRequest.h \
           NMessageAck.h \
           NMessageUnsubscribe.h \
           NMessage.h \
           NMessageNegotiateConnection.h \
           NMessageNegotiateConnectionResponse.h \
           NMessageUnregisterCompoundMethod.h  \
           NMessageNotification.h \
           NMessageRegisterCompoundMethod.h \
           NWebSocketServer.h \
    NWebSocket.h

SOURCES += NMessageResponse.cpp \
           NMessageRequest.cpp \
           NMessageAck.cpp \
           NMessageUnsubscribe.cpp \
           NMessage.cpp \
           NMessageNegotiateConnection.cpp \
           NMessageNegotiateConnectionResponse.cpp \
           NMessageUnregisterCompoundMethod.cpp  \
           NMessageNotification.cpp \
           NMessageRegisterCompoundMethod.cpp \
           NWebSocketServer.cpp \
    NWebSocket.cpp

# Dependencies

