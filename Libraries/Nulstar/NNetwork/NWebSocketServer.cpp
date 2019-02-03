#include <QWebSocket>
#include <QVersionNumber>

#include "NMessage.h"
#include "NMessageNegotiateConnection.h"
#include "NMessageNegotiateConnectionResponse.h"
#include "NMessageRequest.h"
#include "NWebSocket.h"
#include "NWebSocketServer.h"

namespace NulstarNS {
  NWebSocketServer::NWebSocketServer(const QString& lName, const QString& lLabel, SslMode lSslMode, const QList<QVersionNumber> &lProtocolVersionsSupported, QObject* rParent)
                  : QWebSocketServer(lLabel, lSslMode, rParent), mMaxConnections(0), mPort(0), mProtocolVersionsSupported(lProtocolVersionsSupported), mName(lName), mBindAddress(QHostAddress::Null) {

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
    rSocket->fSetName(QString::number(lMSecsSinceEpoch));
    mConnections[lMSecsSinceEpoch] = rSocket;
  }

  void NWebSocketServer::fProcessTextMessage(const QString& lMessage) {
    NWebSocket* rClient = qobject_cast<NWebSocket*>(sender());
    if(rClient) {
    //  emit fTextMessageArrived(rClient->fModuleName(), lMessage);
qDebug() << "Text Message received:" << lMessage;
      QString lMessageType;
      QJsonObject lMessageObject(NMessageFactory::fMessageObjectFromString(lMessage, &lMessageType));
      if(lMessageType == cTypeNegotiateConnection && NMessageNegotiateConnection::fValidateMessageObject(lMessageObject))
        fProcessNegotiateConnection(lMessageObject, rClient);
  /*    if(lMessageType == cTypeNegotiateConnectionResponse && NMessageNegotiateConnectionResponse::fValidateMessageObject(lMessageObject))
        fProcessNegotiateConnectionResponse(lMessageObject, rClient); // Request is always received in NWebSocket*/
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
      fRemoveConnections(QList<qint64> () << lSocketID);
    }
  }

  void NWebSocketServer::fRemoveConnections(const QList<qint64>& lConnectionIDList) {
    for(qint64 lConnectionID : lConnectionIDList) {
      if(mMessageQueue.contains(lConnectionID)) {
        QMap<QString, NMessage* > lMessages = mMessageQueue.value(lConnectionID);
        QMapIterator<QString, NMessage*> i1(lMessages);
        while(i1.hasNext()) {
          i1.next();
          delete i1.value();
        }
        mMessageQueue.remove(lConnectionID);
      }
      mConnections[lConnectionID]->close();
      mConnections[lConnectionID]->deleteLater();
      mConnections.remove(lConnectionID);
    }
  }

  bool NWebSocketServer::fVersionSupported(const QJsonObject& lObjectMessage) {
    QVersionNumber lIncommingVersion = QVersionNumber::fromString(lObjectMessage.value(cMessageDataFieldName).toObject().value(cProtocolVersionFieldName).toString());
qDebug() << QString("Inc Version: '%1'").arg(lIncommingVersion.toString());
    for(const QVersionNumber& lVersionSupported : mProtocolVersionsSupported) {
      if(lIncommingVersion == lVersionSupported) {
        return true;
      }
    }
qDebug() << QString("Protocol Version '%1' not supported!").arg(lIncommingVersion.toString());
    return false;
  }

  void NWebSocketServer::fProcessNegotiateConnection(const QJsonObject& lObjectMessage, NWebSocket* rConnection) {
    if(!rConnection) {
      qDebug() << QString("Connection '%1' no longer exists!").arg(rConnection->fName());
      return;
    }
    if(fVersionSupported(lObjectMessage)) {
      NMessageNegotiateConnectionResponse rNegotiationResponse(rConnection->fName(), QString(), NMessageNegotiateConnectionResponse::ENegotiationStatus::eNegotiationSuccessful, QString("Negotiation successful!"));
      QString lJsonMessage(rNegotiationResponse.fToJsonString());
       rConnection->fSetConnectionState(NWebSocket::EConnectionState::eConnectionActive);
       qint64 lBytesSent = rConnection->sendTextMessage(lJsonMessage);
 qDebug() << QString("Bytes Sent: '%1'\n  - Message: '%2' Error> %3").arg(QString::number(lBytesSent)).arg(lJsonMessage).arg(rConnection->errorString());
    }
    else {
      NMessageNegotiateConnectionResponse lNegotiationResponse(rConnection->fName(), QString(), NMessageNegotiateConnectionResponse::ENegotiationStatus::eNegotiationError, QString("Negotiation unsuccessful! Protocol Version not supported!"));
      QString lJsonMessage(lNegotiationResponse.fToJsonString());
 qDebug() << QString("Message Sent: '%1'").arg(lJsonMessage);
      rConnection->sendTextMessage(lJsonMessage);
      fRemoveConnections(QList<qint64> () << rConnection->fName().toLongLong());
    }
  }

  void NWebSocketServer::fSetBindAddress(const QHostAddress& lBindAddress) {
    if(lBindAddress != QHostAddress::Null) mBindAddress = lBindAddress;
  }

  void NWebSocketServer::fSetPort(quint16 lPort) {
    if(lPort) mPort = lPort;
  }

  NWebSocket* NWebSocketServer::fNextPendingConnection() {
    QWebSocket* rPendingConnection = nextPendingConnection();
    if(rPendingConnection) {
      NWebSocket* rConnection = new NWebSocket(rPendingConnection);
      connect(rPendingConnection, &QWebSocket::textMessageReceived, rConnection, &NWebSocket::textMessageReceived, Qt::UniqueConnection);
      return rConnection;
    }
    return nullptr;
  }
}
