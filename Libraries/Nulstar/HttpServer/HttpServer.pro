# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings

QT -= gui
QT += network websockets
CONFIG += c++11 skip_target_version_ext

TEMPLATE = lib
TARGET = QtHttpServer
VERSION = $$NULSTAR_LIBRARIES_VERSION
DEFINES += QT_BUILD_HTTPSERVER_LIB

DESTDIR=$$LIBRARIES_OUTDIR

QT = network core-private
QT += websockets-private

# Source Files

HEADERS += \
    http_parser.h \
    qthttpserverglobal.h \
    qabstracthttpserver.h \
    qabstracthttpserver_p.h \
    qhttpserver.h \
    qhttpserver_p.h \
    qhttpserverrequest.h \
    qhttpserverrequest_p.h \
    qhttpserverresponder.h \
    qhttpserverresponder_p.h \
    qhttpserverresponse.h \
    qhttpserverresponse_p.h \
    qhttpserverrouter.h \
    qhttpserverrouter_p.h \
    qhttpserverrouterrule.h \
    qhttpserverrouterrule_p.h \
    qhttpserverrouterviewtraits.h

SOURCES += \
    http_parser.c \
    qabstracthttpserver.cpp \
    qhttpserver.cpp \
    qhttpserverrequest.cpp \
    qhttpserverresponder.cpp \
    qhttpserverresponse.cpp \
    qhttpserverrouter.cpp \
    qhttpserverrouterrule.cpp

# Dependencies

INCLUDEPATH += .
LIBS += -lNNetwork -lNTools
