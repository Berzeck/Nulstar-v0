#include "NConnectionServer.h"

#include <QWebSocket>
#include <QWebSocketServer>

namespace NulstarNS {
  NConnectionServer::NConnectionServer(bool lSecureMode, quint16 lPort, quint8 lDebugLevel, QObject* rParent)
                      : QObject(rParent), mSecureMode(lSecureMode), mDebugLevel(lDebugLevel) {
    if(!mSecureMode) pWebSocketServer = new QWebSocketServer(QStringLiteral("Nulstar"), QWebSocketServer::NonSecureMode, this);
    if(pWebSocketServer->listen(QHostAddress::Any, lPort)) {
      connect(pWebSocketServer, &QWebSocketServer::newConnection, this, &NConnectionServer::fOnNewConnection);
      connect(pWebSocketServer, &QWebSocketServer::closed, this, &NConnectionServer::sClosed);
    }
  }

  NConnectionServer::~NConnectionServer() {
    pWebSocketServer->close();
    qDeleteAll(mClients.begin(), mClients.end());
  }

  void NConnectionServer::fOnNewConnection() {
      QWebSocket* rSocket = pWebSocketServer->nextPendingConnection();
      connect(rSocket, &QWebSocket::textMessageReceived, this, &NConnectionServer::fProcessTextMessage);
      connect(rSocket, &QWebSocket::binaryMessageReceived, this, &NConnectionServer::fProcessBinaryMessage);
      connect(rSocket, &QWebSocket::disconnected, this, &NConnectionServer::fSocketDisconnected);
      mClients << rSocket;
  }

  void NConnectionServer::fProcessTextMessage(QString lMessage) {
    QWebSocket* rClient = qobject_cast<QWebSocket* >(sender());
         qDebug() << "Message received:" << lMessage;
    if (rClient) rClient->sendTextMessage(QString("Nulstar - ") + lMessage );
  }

  void NConnectionServer::fProcessBinaryMessage(QByteArray lMessage) {
    QWebSocket *rClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Binary Message received:" << lMessage;
    if (rClient) rClient->sendBinaryMessage(lMessage);
  }

  void NConnectionServer::fSocketDisconnected() {
      QWebSocket *rClient = qobject_cast<QWebSocket *>(sender());
       qDebug() << "socketDisconnected:" << rClient;
      if (rClient) {
          mClients.removeAll(rClient);
          rClient->deleteLater();
      }
  }
}

