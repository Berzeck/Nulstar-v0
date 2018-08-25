#include <QHostAddress>
#include <QWebSocket>
#include <QWebSocketServer>
#include <NWebSocketsServer.h>
#include "NConnectionController.h"


namespace NulstarNS {
  NConnectionController::NConnectionController(quint8 lSecureMode, quint8 lDebugLevel, QObject* rParent)
                       : NCoreService (rParent), mSecureMode(lSecureMode), mDebugLevel(lDebugLevel) {
    if(lSecureMode == NWebSocketsServer::NonSecureMode) {
      pWebAdminServer = new NWebSocketsServer(QStringLiteral("Nulstar Admin"), NWebSocketsServer::NonSecureMode, this);
      pWebClientServer = new NWebSocketsServer(QStringLiteral("Nulstar Client"), NWebSocketsServer::NonSecureMode, this);
      pWebCommServer = new NWebSocketsServer(QStringLiteral("Nulstar Internal Communication"), NWebSocketsServer::NonSecureMode, this);
    }
  }

  NConnectionController::~NConnectionController() {
    pWebAdminServer->close();
    pWebClientServer->close();    
    pWebCommServer->close();
  }

  void NConnectionController::fControlAdminServer(bool lStartServer, quint16 lAdminPort) {
    if(lAdminPort && lStartServer) pWebAdminServer->fListen(QHostAddress::Any, lAdminPort);
    else pWebAdminServer->close();
  }

  void NConnectionController::fControlClientServer(bool lStartServer, quint16 lClientPort) {
    if(lClientPort && lStartServer) pWebClientServer->fListen(QHostAddress::Any, lClientPort);
    else pWebClientServer->close();
  }

  void NConnectionController::fControlCommServer(bool lStartServer, quint16 lCommPort) {
    if(lCommPort && lStartServer) pWebCommServer->fListen(QHostAddress::Any, lCommPort);
    else pWebCommServer->close();
  }
}

