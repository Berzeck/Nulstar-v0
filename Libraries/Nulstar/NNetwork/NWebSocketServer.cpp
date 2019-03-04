#include <QDateTime>
#include <QWebSocket>
#include <QVersionNumber>

#include "NMessageNegotiateConnection.h"
#include "NMessageNegotiateConnectionResponse.h"
#include "NWebSocket.h"
#include "NWebSocketServer.h"

namespace NulstarNS {
  NWebSocketServer::NWebSocketServer(const QString& lName, const QString& lLabel, SslMode lSslMode, const QList<QVersionNumber> &lProtocolVersionsSupported, QObject* rParent)
                  : QWebSocketServer(lLabel, lSslMode, rParent), mMaxConnections(0), mPort(0), mProtocolVersionsSupported(lProtocolVersionsSupported), mName(lName), mBindAddress(QHostAddress::Null) {

  }

  NWebSocketServer::~NWebSocketServer() {      
    fRemoveConnections();
  }

  bool NWebSocketServer::fListen(const QHostAddress& lBindAddress, quint16 lPort) {
    bool lSuccess = false;
    if(isListening())
      return false;
    fSetPort(lPort);
    fSetBindAddress(lBindAddress);
    lSuccess = listen(mBindAddress, mPort);
    if(lSuccess)
      connect(this, &NWebSocketServer::newConnection, this, &NWebSocketServer::fOnNewConnection, static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::AutoConnection));
    return lSuccess;
  }

  void NWebSocketServer::fOnNewConnection() {
    if(mMaxConnections && (mConnections.size() >= mMaxConnections))
      return;
    QWebSocket* rPendingConnection = nextPendingConnection();

    if(rPendingConnection != nullptr) {
      qint64 lMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch());
      NWebSocket* rSocket = new NWebSocket(QString::number(lMSecsSinceEpoch), QString(), QUrl(), 0, rPendingConnection, this);
      connect(rSocket, &NWebSocket::sMessageReceived, this, &NWebSocketServer::fProcessTextMessage, Qt::UniqueConnection);
      connect(rSocket, &NWebSocket::sDisconnected, this, &NWebSocketServer::fSocketDisconnected, Qt::UniqueConnection);
      mConnections[lMSecsSinceEpoch] = rSocket;
    }
  }
  void NWebSocketServer::fSendMessage(const qint64 lWebSocketID, NMessage* rMessage) {
    if(mConnections.contains(lWebSocketID)) {
      mConnections.value(lWebSocketID)->fSendMessage(rMessage);
    }
    else {
      qDebug("%s", qUtf8Printable(QString("Message '%1' couldn't be sent because WebSocket '%2' wasn't found!").arg(rMessage->fMessageID()).arg(lWebSocketID)));
    }
  }

  void NWebSocketServer::fProcessTextMessage(const QString& lMessageType, const QVariantMap& lMessage) {
    NWebSocket* rSocket = qobject_cast<NWebSocket*>(sender());
    if(rSocket) {
      if(lMessageType == cTypeNegotiateConnection)
        fProcessNegotiateConnection(lMessage, rSocket);
  /*    if(lMessageType == cTypeNegotiateConnectionResponse && NMessageNegotiateConnectionResponse::fValidateMessageObject(lMessageObject))
        fProcessNegotiateConnectionResponse(lMessageObject, rSocket); // Request is always received in NWebSocket*/
      if(lMessageType == cTypeRequest)
        fProcessRequest(lMessage, rSocket);
    }
  }

  void NWebSocketServer::fSocketDisconnected() {
    NWebSocket* rSocket = qobject_cast<NWebSocket*>(sender());
qDebug() << "socketDisconnected:" << rSocket->fName();
    if(rSocket) {
      fRemoveConnections(QList<qint64> () << rSocket->fName().toLongLong());
      emit sWebSocketDisconnected(rSocket->fName());
    }
  }

  void NWebSocketServer::fRemoveConnections(const QList<qint64>& lConnectionIDList) {
    QList<qint64> lConnectionList(lConnectionIDList);
    if(lConnectionList.size() == 0)
       lConnectionList = mConnections.keys();
    for(qint64 lConnectionID : lConnectionList) {
      if(mMessageQueue.contains(lConnectionID)) {
        QMap<QString, NMessage* > lMessages = mMessageQueue.value(lConnectionID);
        QMapIterator<QString, NMessage*> i1(lMessages);
        while(i1.hasNext()) {
          i1.next();
          i1.value()->deleteLater();
        }
        mMessageQueue.remove(lConnectionID);
      }
      mConnections[lConnectionID]->fClose();
      mConnections[lConnectionID]->deleteLater();
      mConnections.remove(lConnectionID);
    }
  }

  bool NWebSocketServer::fVersionSupported(const QVariantMap &lMessage) {
    QVersionNumber lIncommingVersion = QVersionNumber::fromString(lMessage.value(cFieldName_MessageData).toMap().value(cProtocolVersionFieldName).toString());
qDebug() << QString("Inc Version: '%1'").arg(lIncommingVersion.toString());
    for(const QVersionNumber& lVersionSupported : mProtocolVersionsSupported) {
      if(lIncommingVersion == lVersionSupported) {
        return true;
      }
    }
qDebug() << QString("Protocol Version '%1' not supported!").arg(lIncommingVersion.toString());
    return false;
  }

  void NWebSocketServer::fProcessNegotiateConnection(const QVariantMap& lMessage, NWebSocket* rConnection) {
    if(!rConnection) {
      qDebug() << QString("Connection '%1' no longer exists!").arg(rConnection->fName());
      return;
    }
    if(fVersionSupported(lMessage)) {
      NMessageNegotiateConnectionResponse rNegotiationResponse(rConnection->fName(), QString(), NMessageNegotiateConnectionResponse::ENegotiationStatus::eNegotiationSuccessful, QString("Negotiation successful!"));
      QString lJsonMessage(rNegotiationResponse.fToJsonString());
       rConnection->fSetConnectionState(NWebSocket::EConnectionState::eConnectionActive);
       qint64 lBytesSent = rConnection->fSendTextMessage(lJsonMessage);

 qDebug() << QString("Bytes Sent: '%1'\n  - Message: '%2'").arg(QString::number(lBytesSent)).arg(lJsonMessage);
    }
    else {
      NMessageNegotiateConnectionResponse lNegotiationResponse(rConnection->fName(), QString(), NMessageNegotiateConnectionResponse::ENegotiationStatus::eNegotiationError, QString("Negotiation unsuccessful! Protocol Version not supported!"));
      QString lJsonMessage(lNegotiationResponse.fToJsonString());
 //qDebug() << QString("Message Sent: '%1'").arg(lJsonMessage) << rConnection->state();
      rConnection->fSendTextMessage(lJsonMessage);
      fRemoveConnections(QList<qint64> () << rConnection->fName().toLongLong());
    }
  }

  void NWebSocketServer::fProcessRequest(const QVariantMap &lMessage, NWebSocket* rConnection) {
    if(!rConnection) {
      qDebug() << QString("Connection '%1' no longer exists!").arg(rConnection->fName());
      return;
    }

    QVariantMap lRequestMethods(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_RequestMethods).toMap());
    QString lMessageID(lMessage.value(cFieldName_MessageID).toString());
    quint64 lSubscriptionEventCounter(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionEventCounter).toULongLong());
    quint64 lSubscriptionPeriod(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionPeriod).toULongLong());
    for(const QString& lRequestMethodName : lRequestMethods.keys()) {
      QVariantMap lRequestMethodParams = lRequestMethods.value(lRequestMethodName).toMap();
      TMessageRequestToProcess tMessageRequest({fName(), rConnection->fName(),lMessageID, lRequestMethodName, lRequestMethodParams, lSubscriptionEventCounter, lSubscriptionPeriod, 0, 0} );
      emit sRequestMessageArrived(tMessageRequest);
    }
  }

  void NWebSocketServer::fSetBindAddress(const QHostAddress& lBindAddress) {
    if(lBindAddress != QHostAddress::Null) mBindAddress = lBindAddress;
  }

  void NWebSocketServer::fSetPort(quint16 lPort) {
    if(lPort) mPort = lPort;
  }

}
