#include <QHostAddress>
#include <QWebSocket>
#include <QWebSocketServer>
#include <NNulstarWebSocketsServer.h>
#include "NConnectionController.h"


namespace NulstarNS {
  NConnectionController::NConnectionController(quint8 lSecureMode, quint16 lAdminPort, quint16 lClientPort, quint16 lCommPort, quint8 lDebugLevel, QObject* rParent)
                       : QObject(rParent), mSecureMode(lSecureMode), mAdminPort(lAdminPort), mClientPort(lClientPort), mCommPort(lCommPort), mDebugLevel(lDebugLevel) {
    if(lSecureMode == NNulstarWebSocketsServer::NonSecureMode) {
      pWebAdminServer = new NNulstarWebSocketsServer(QStringLiteral("Nulstar Admin"), NNulstarWebSocketsServer::NonSecureMode, this);
      pWebClientServer = new NNulstarWebSocketsServer(QStringLiteral("Nulstar Client"), NNulstarWebSocketsServer::NonSecureMode, this);
      pWebCommServer = new NNulstarWebSocketsServer(QStringLiteral("Nulstar Internal Communication"), NNulstarWebSocketsServer::NonSecureMode, this);
    }
  }

  NConnectionController::~NConnectionController() {
    pWebAdminServer->close();
    pWebClientServer->close();    
    pWebCommServer->close();
  }

  void NConnectionController::fControlAdminServer(bool lStartServer) {
    if(lStartServer) pWebAdminServer->listen(QHostAddress::Any, mAdminPort);

  }

  void NConnectionController::fControlClientServer(bool lStartServer) {
    if(lStartServer) pWebAdminServer->listen(QHostAddress::Any, mAdminPort);
  }

  void NConnectionController::fControlCommServer(bool lStartServer) {
    if(lStartServer) pWebAdminServer->listen(QHostAddress::Any, mAdminPort);
  }
}

