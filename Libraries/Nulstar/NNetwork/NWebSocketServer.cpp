#include <QWebSocket>
#include <NMessage.h>

#include "NMessageRequest.h"
#include "NWebSocketServer.h"

namespace NulstarNS {
  NWebSocketServer::NWebSocketServer(const QString& lName, const QString& lLabel, SslMode lSslMode, QObject* rParent)
                  : QWebSocketServer(lLabel, lSslMode, rParent), mMaxConnections(0), mPort(0), mName(lName), mBindAddress(QHostAddress::Null) {

  }

  NWebSocketServer::~NWebSocketServer() {      
    QMapIterator<qint64, QWebSocket*> i1(mConnections);
    while(i1.hasNext()) {
      i1.next();
      i1.value()->deleteLater();
    }
  }

  bool NWebSocketServer::fListen(const QHostAddress& lBindAddress, quint16 lPort) {
    bool lSuccess = false;
    if(isListening())
      return false;
    fSetPort(lPort);
    fSetBindAddress(lBindAddress);
    lSuccess = listen(mBindAddress, mPort);
    if(lSuccess)
      connect(this, &NWebSocketServer::QWebSocketServer::newConnection, this, &NWebSocketServer::fOnNewConnection, static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::AutoConnection));
    return lSuccess;
  }

  void NWebSocketServer::fOnNewConnection() {
    if(mMaxConnections && (mConnections.size() >= mMaxConnections))
      return;
    QWebSocket* rSocket = nextPendingConnection();
    connect(rSocket, &QWebSocket::textMessageReceived, this, &NWebSocketServer::fProcessTextMessage);
    connect(rSocket, &QWebSocket::binaryMessageReceived, this, &NWebSocketServer::fProcessBinaryMessage);
    connect(rSocket, &QWebSocket::disconnected, this, &NWebSocketServer::fSocketDisconnected);
    qint64 lMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch());
    rSocket->setProperty("ID", lMSecsSinceEpoch);
    mConnections[lMSecsSinceEpoch] = rSocket;
  }

  void NWebSocketServer::fProcessTextMessage(QString lMessage) {    
    emit fTextMessageArrived(mName, lMessage);
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
      qint64 lSocketID = rClient->property("ID").toLongLong();
      mConnections.remove(lSocketID);
      rClient->deleteLater();
    }
  }

  void NWebSocketServer::fSetBindAddress(const QHostAddress& lBindAddress) {
    if(lBindAddress != QHostAddress::Null) mBindAddress = lBindAddress;
  }

  void NWebSocketServer::fSetPort(quint16 lPort) {
    if(lPort) mPort = lPort;
  }
}
