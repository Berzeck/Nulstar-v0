#include <QJsonDocument>
#include <QTimer>
#include <QWebSocket>

#include "NWebSocket.h"
#include "NMessageFactory.h"
#include "NMessageNegotiateConnection.h"
#include "NMessageNegotiateConnectionResponse.h"
#include "NMessageRequest.h"
#include "NMessageResponse.h"

namespace NulstarNS {
  NWebSocket::NWebSocket(const QString &lName, const QString &lProtocolVersion, const QUrl& lConnectionUrl, quint8 lConnectionRetryInterval, QWebSocket *rSocket, QObject* rParent)
            : QObject(rParent), mConnectionRetryInterval(lConnectionRetryInterval), mConnectionState(EConnectionState::eConnectionNotNegotiated),
              mName(lName), mProtocolVersion(lProtocolVersion), mConnectionUrl(lConnectionUrl) {
    if(rSocket != nullptr)
      mWebSocket = rSocket;
    else
      mWebSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
    connect(mWebSocket, &QWebSocket::connected, this, &NWebSocket::fOnConnected);
    connect(mWebSocket, &QWebSocket::disconnected, this, &NWebSocket::fOnSocketDisconnection);
    connect(mWebSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &NWebSocket::fOnConnectionError);
    connect(mWebSocket, &QWebSocket::textMessageReceived, this, &NWebSocket::fOnTextMessageReceived);

    pRetryTimer = new QTimer(this);
    connect(pRetryTimer, &QTimer::timeout, this, &NWebSocket::fConnect);
  }

  void NWebSocket::fConnect() {
    if(mWebSocket->state() == QAbstractSocket::UnconnectedState && mConnectionUrl.isValid()) {
      mWebSocket->open(mConnectionUrl);
    }
  }

  void NWebSocket::fQueueMessage(NMessage* rMessage, EConnectionState lMinStateRequired) {
    mMessageQueue.append(rMessage);
    mMessageResponses.insert(rMessage->fMessageID().toULongLong(), rMessage);
    rMessage->fSetStatus(NMessage::EMessageStatus::eAwaitingDelivery);
    if((lMinStateRequired <= fConnectionState()) && (mWebSocket->state() == QAbstractSocket::ConnectedState))
      fSendMessage(rMessage);
    else {
      rMessage->deleteLater();
      emit sLog(ELogLevel::eLogWarning, ELogMessageType::eMessageSent, rMessage->fToJsonString());
    }
  }

  void NWebSocket::fSendMessage(NMessage* rMessage) {
    if(!rMessage)
      return;
    qint64 lBytesSent = mWebSocket->sendTextMessage(rMessage->fToJsonString());
  //qDebug("%s \n\n", qUtf8Printable(rMessage->fToJsonString()));
    if(lBytesSent) {
      rMessage->fSetStatus(NMessage::EMessageStatus::eSent);
      emit sLog(ELogLevel::eLogInfo, ELogMessageType::eMessageSent, rMessage->fToJsonString());
    }
    else {
      rMessage->fSetStatus(NMessage::EMessageStatus::eWithErrorAndWitheld);
      emit sLog(ELogLevel::eLogWarning, ELogMessageType::eMessageSent, rMessage->fToJsonString());
    }
    rMessage->deleteLater();
  }

  void NWebSocket::fOnConnectionError(QAbstractSocket::SocketError lErrorCode) {
    qDebug("%s", qUtf8Printable(QString::number(lErrorCode)));
    if(pRetryTimer && mConnectionRetryInterval)
      pRetryTimer->start(mConnectionRetryInterval * 1000);
    emit sError(lErrorCode);
  }

  void NWebSocket::fOnConnected() {
    if(fConnectionState() == NWebSocket::EConnectionState::eConnectionNotNegotiated)
      fNegotiateConnection();
    if(pRetryTimer)
      pRetryTimer->stop();
    emit sConnected();
  }

  void NWebSocket::fOnSocketDisconnection() {
    fSetConnectionState(NWebSocket::EConnectionState::eConnectionNotNegotiated);
    if(pRetryTimer && mConnectionRetryInterval)
      pRetryTimer->start(mConnectionRetryInterval * 1000);
    emit sDisconnected();
  }

  void NWebSocket::fOnTextMessageReceived(const QString& lMessage) {
    qDebug() << "\nText Message received:" << lMessage;
    QString lMessageType;
    QJsonObject lMessageObject(NMessageFactory::fMessageObjectFromString(lMessage, &lMessageType));    
    if(lMessageType == cTypeNegotiateConnectionResponse && NMessageNegotiateConnectionResponse::fValidateMessageObject(lMessageObject))
      fProcessNegotiateConnectionResponse(lMessageObject);
    if(lMessageType == cTypeReponse && NMessageResponse::fValidateMessageObject(lMessageObject))
      emit sMessageReceived(lMessageType, lMessageObject.toVariantMap());
    if(lMessageType == cTypeNegotiateConnection && NMessageNegotiateConnection::fValidateMessageObject(lMessageObject))
      emit sMessageReceived(lMessageType, lMessageObject.toVariantMap());
    if(lMessageType == cTypeRequest && NMessageRequest::fValidateMessageObject(lMessageObject))
      emit sMessageReceived(lMessageType, lMessageObject.toVariantMap());

    emit sLog(ELogLevel::eLogInfo, ELogMessageType::eMessageReceived, lMessage);
 }

  void NWebSocket::fNegotiateConnection() {
    fQueueMessage(new  NMessageNegotiateConnection(mName, QString(), mProtocolVersion, 0, NMessageNegotiateConnection::ECompressionAlgorithm::eZlib, this), EConnectionState::eConnectionNotNegotiated);
  }

  void NWebSocket::fProcessNegotiateConnectionResponse(const QJsonObject& lObjectMessage) {
    QJsonObject lDataObject(lObjectMessage.value(cFieldName_MessageData).toObject());
    if(lDataObject.value(cNegotiationStatusFieldName).toVariant().toBool()) {
      fSetConnectionState(NWebSocket::EConnectionState::eConnectionActive);
      //qDebug() << QString("Negotiation attempt to '%1' successful!").arg(fName());
    }
    else {
 qDebug() << QString("Negotiation attempt to '%1' failed!").arg(fName());
    }
  }

  void NWebSocket::fRegisterApi(const QVariantMap& lApiMap) {
    fQueueMessage(new  NMessageRequest(mName, QString(), false, 0, 0, QString(), 0, lApiMap, this), EConnectionState::eConnectionActive);
  }
}
