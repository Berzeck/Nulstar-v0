#include <QWebSocket>
#include "NWebSocketServer.h"

namespace NulstarNS {
  NWebSocketServer::NWebSocketServer(const QString& lName, SslMode lSslMode, QObject* rParent)
                  : QWebSocketServer(lName, lSslMode, rParent), mPort(0) {

  }

  NWebSocketServer::~NWebSocketServer() {
    qDeleteAll(mConnections.begin(), mConnections.end());
  }

  bool NWebSocketServer::fListen(const QHostAddress& lAddress, quint16 lPort) {
    bool lSuccess = false;
    if(isListening())
      return false;
    if(lPort) mPort = lPort;
    if((lSuccess = listen(lAddress, mPort)))
      connect(this, &NWebSocketServer::QWebSocketServer::newConnection, this, &NWebSocketServer::fOnNewConnection, static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::AutoConnection));
    return lSuccess;
  }

  void NWebSocketServer::fOnNewConnection() {
    QWebSocket* rSocket = nextPendingConnection();
    connect(rSocket, &QWebSocket::textMessageReceived, this, &NWebSocketServer::fProcessTextMessage);
    connect(rSocket, &QWebSocket::binaryMessageReceived, this, &NWebSocketServer::fProcessBinaryMessage);
    connect(rSocket, &QWebSocket::disconnected, this, &NWebSocketServer::fSocketDisconnected);
    mConnections << rSocket;
  }

  void NWebSocketServer::fProcessTextMessage(QString lMessage) {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "Message received:" << lMessage;
    if (rClient) rClient->sendTextMessage(QString("Nulstar - ") + lMessage );
  }

  void NWebSocketServer::fProcessBinaryMessage(QByteArray lMessage) {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "Binary Message received:" << lMessage;
    if (rClient) rClient->sendBinaryMessage(lMessage);
  }

  void NWebSocketServer::fSocketDisconnected() {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "socketDisconnected:" << rClient;
    if(rClient) {
      mConnections.removeAll(rClient);
      rClient->deleteLater();
    }
  }

  void NWebSocketServer::fSetPort(quint16 lPort) {
    if(lPort) mPort = lPort;
  }
}
