#include <QWebSocket>
#include "NNulstarWebSocketsServer.h"

namespace NulstarNS {
  NNulstarWebSocketsServer::NNulstarWebSocketsServer(const QString& lName, SslMode lSecureMode, QObject* rParent)
                          : QWebSocketServer(lName, lSecureMode, rParent) {

  }

  NNulstarWebSocketsServer::~NNulstarWebSocketsServer() {
    qDeleteAll(mConnections.begin(), mConnections.end());
  }
  bool NNulstarWebSocketsServer::fListen(const QHostAddress& lAddress, quint16 lPort) {
    bool lSuccess = false;
    if(isListening())
      return true;
    if((lSuccess = listen(lAddress, lPort)))
      connect(this, &NNulstarWebSocketsServer::QWebSocketServer::newConnection, this, &NNulstarWebSocketsServer::fOnNewConnection, static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::AutoConnection));
    return lSuccess;
  }

  void NNulstarWebSocketsServer::fOnNewConnection() {
    QWebSocket* rSocket = nextPendingConnection();
    connect(rSocket, &QWebSocket::textMessageReceived, this, &NNulstarWebSocketsServer::fProcessTextMessage);
    connect(rSocket, &QWebSocket::binaryMessageReceived, this, &NNulstarWebSocketsServer::fProcessBinaryMessage);
    connect(rSocket, &QWebSocket::disconnected, this, &NNulstarWebSocketsServer::fSocketDisconnected);
    mConnections << rSocket;
  }

  void NNulstarWebSocketsServer::fProcessTextMessage(QString lMessage) {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "Message received:" << lMessage;
    if (rClient) rClient->sendTextMessage(QString("Nulstar - ") + lMessage );
  }

  void NNulstarWebSocketsServer::fProcessBinaryMessage(QByteArray lMessage) {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "Binary Message received:" << lMessage;
    if (rClient) rClient->sendBinaryMessage(lMessage);
  }

  void NNulstarWebSocketsServer::fSocketDisconnected() {
    QWebSocket* rClient = qobject_cast<QWebSocket*>(sender());
qDebug() << "socketDisconnected:" << rClient;
    if(rClient) {
      mConnections.removeAll(rClient);
      rClient->deleteLater();
    }
  }
}
