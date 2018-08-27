#include <QHostAddress>
#include <QWebSocket>
#include <QWebSocketServer>
#include <NWebSocketsServer.h>
#include "NServiceManagerController.h"


namespace NulstarNS {
  NServiceManagerController::NServiceManagerController(quint8 lSecureMode, quint8 lDebugLevel, QObject* rParent)
                           : NCoreService (rParent), mSecureMode(lSecureMode), mDebugLevel(lDebugLevel) {
    if(lSecureMode == NWebSocketsServer::NonSecureMode) {
      pWebCommServer = new NWebSocketsServer(QStringLiteral("Nulstar Internal Communication"), NWebSocketsServer::NonSecureMode, this);
    }
  }

  NServiceManagerController::~NServiceManagerController() {
    pWebCommServer->close();
  }

  void NServiceManagerController::fControlCommServer(bool lStartServer, quint16 lCommPort) {
    if(lCommPort && lStartServer) pWebCommServer->fListen(QHostAddress::Any, lCommPort);
    else pWebCommServer->close();
  }
}

