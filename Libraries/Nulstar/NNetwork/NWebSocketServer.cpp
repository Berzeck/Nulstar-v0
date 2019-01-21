#include <QWebSocket>
#include <NMessage.h>

#include "NMessageRequest.h"
#include "NWebSocket.h"
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
    NWebSocket* rSocket = fNextPendingConnection();
    connect(rSocket, &NWebSocket::textMessageReceived, this, &NWebSocketServer::fProcessTextMessage);
    connect(rSocket, &NWebSocket::binaryMessageReceived, this, &NWebSocketServer::fProcessBinaryMessage);
    connect(rSocket, &NWebSocket::disconnected, this, &NWebSocketServer::fSocketDisconnected);
    qint64 lMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch());
    rSocket->fSetModuleName(QString::number(lMSecsSinceEpoch));
    mConnections[lMSecsSinceEpoch] = rSocket;
  }

  void NWebSocketServer::fProcessTextMessage(const QString& lMessage) {
    NWebSocket* rClient = qobject_cast<NWebSocket*>(sender());
    if(rClient) {
      emit fTextMessageArrived(rClient->fModuleName(), lMessage);
      QString lMessageType;
      QJsonObject lMessageObject(NMessageFactory::fMessageObjectFromString(lMessage, &lMessageType));
      if(lMessageType == cTypeNegotiateConnection) {

      }
    }
  }

  void NWebSocketServer::fProcessBinaryMessage(QByteArray lMessage) {
    NWebSocket* rClient = qobject_cast<NWebSocket*>(sender());
qDebug() << "Binary Message received:" << lMessage;
    if (rClient) rClient->sendBinaryMessage(lMessage);
  }

  void NWebSocketServer::fSocketDisconnected() {
    NWebSocket* rClient = qobject_cast<NWebSocket*>(sender());
qDebug() << "socketDisconnected:" << rClient;
    if(rClient) {
      qint64 lSocketID = rClient->property("ID").toLongLong();
      mConnections.remove(lSocketID);
      if(mMessageQueue.contains(lSocketID)) {
        QMap<QString, NMessage* > lMessages;
        QMapIterator<QString, NMessage*> i1(lMessages);
        while(i1.hasNext()) {
          i1.next();
          delete i1.value();
        }
        mMessageQueue.remove(lSocketID);
      }
      rClient->deleteLater();
    }
  }

  void NWebSocketServer::fSetBindAddress(const QHostAddress& lBindAddress) {
    if(lBindAddress != QHostAddress::Null) mBindAddress = lBindAddress;
  }

  void NWebSocketServer::fSetPort(quint16 lPort) {
    if(lPort) mPort = lPort;
  }

  NWebSocket* NWebSocketServer::fNextPendingConnection() {
    NWebSocket* rConnection = new NWebSocket(nextPendingConnection());
    return rConnection;
  }
}
