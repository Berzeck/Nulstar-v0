#include <QWebSocket>
#include "NWebSocketsServer.h"

namespace NulstarNS {
  NWebSocketsServer::NWebSocketsServer(const QString& lName, SslMode lSecureMode, QObject* rParent)
                          : QWebSocketServer(lName, lSecureMode, rParent) {

  }

  NWebSocketsServer::~NWebSocketsServer() {
    qDeleteAll(mConnections.begin(), mConnections.end());
  }
  bool NWebSocketsServer::fListen(const QHostAddress& lAddress, quint16 lPort) {
    bool lSuccess = false;
    if(isListening())
      return true;
    if((lSuccess = listen(lAddress, lPort)))
      connect(this, &NWebSocketsServer::QWebSocketServer::newConnection, this, &NWebSocketsServer::fOnNewConnection, static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::AutoConnection));
    return lSuccess;
  }

  void NWebSocketsServer::fOnNewConnection() {
    QWebSocket* rSocket = nextPendingConnection();
    connect(rSocket, &QWebSocket::textMessageReceived, this, &NWebSocketsServer::fProcessTextMessage);
    connect(rSocket, &QWebSocket::binaryMessageReceived, this, &NWebSocketsServer::fProcessBinaryMessage);
    connect(rSocket, &QWebSocket::disconnected, this, &NWebSocketsServer::fSocketDisconnected);
    mConnections << rSocket;
  }

  void NWebSocketsServer::fProcessTextMessage(QString lMessage) {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "Message received:" << lMessage;
    if (rClient) rClient->sendTextMessage(QString("Nulstar - ") + lMessage );
  }

  void NWebSocketsServer::fProcessBinaryMessage(QByteArray lMessage) {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "Binary Message received:" << lMessage;
    if (rClient) rClient->sendBinaryMessage(lMessage);
  }

  void NWebSocketsServer::fSocketDisconnected() {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "socketDisconnected:" << rClient;
    if(rClient) {
      mConnections.removeAll(rClient);
      rClient->deleteLater();
    }
  }
}
