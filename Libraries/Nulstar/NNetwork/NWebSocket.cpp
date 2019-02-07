#include <QJsonDocument>
#include <QTimer>
#include <QWebSocket>

#include "NWebSocket.h"
#include "NMessageFactory.h"
#include "NMessageNegotiateConnection.h"
#include "NMessageNegotiateConnectionResponse.h"

namespace NulstarNS {
  NWebSocket::NWebSocket(const QString &lName, const QString &lProtocolVersion, const QUrl& lConnectionUrl, quint8 lConnectionRetryInterval, QWebSocket *rSocket, QObject* rParent)
            : QObject(rParent), mConnectionRetryInterval(lConnectionRetryInterval), mConnectionState(EConnectionState::eConnectionNotNegotiated),
              mName(lName), mProtocolVersion(lProtocolVersion), mConnectionUrl(lConnectionUrl) {
    if(rSocket)
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

  void NWebSocket::fQueueMessage(NMessage* lMessage, EConnectionState lMinStateRequired) {
    mMessageQueue.append(lMessage);
    mMessageResponses.insert(lMessage->fMessageID().toULongLong(), lMessage);
    lMessage->fSetStatus(NMessage::EMessageStatus::eAwaitingDelivery);
    if((lMinStateRequired <= fConnectionState()) && (mWebSocket->state() == QAbstractSocket::ConnectedState))
      fSendMessage(lMessage);
  }

  void NWebSocket::fSendMessage(NMessage* rMessage) {
    qint64 lBytesSent = mWebSocket->sendTextMessage(rMessage->fToJsonString());
    if(lBytesSent)
      rMessage->fSetStatus(NMessage::EMessageStatus::eSent);
    else
      rMessage->fSetStatus(NMessage::EMessageStatus::eWithErrorAndWitheld);
 qDebug("%s", qUtf8Printable(QString::number(mWebSocket->state())));
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
 qDebug() << "Text Message received:" << lMessage;
    QString lMessageType;
    QJsonObject lMessageObject(NMessageFactory::fMessageObjectFromString(lMessage, &lMessageType));
    if(lMessageType == cTypeNegotiateConnectionResponse && NMessageNegotiateConnectionResponse::fValidateMessageObject(lMessageObject))
      fProcessNegotiateConnectionResponse(lMessageObject);
    emit sTextMessageReceived(lMessage);
  }

  void NWebSocket::fNegotiateConnection() {
    fQueueMessage(new  NMessageNegotiateConnection(mName, QString(), mProtocolVersion, 0, NMessageNegotiateConnection::ECompressionAlgorithm::eZlib, this), EConnectionState::eConnectionNotNegotiated);
  }

  void NWebSocket::fProcessNegotiateConnectionResponse(const QJsonObject& lObjectMessage) {
    QJsonObject lDataObject(lObjectMessage.value(cMessageDataFieldName).toObject());
    if(lDataObject.value(cNegotiationStatusFieldName).toVariant().toBool()) {
      fSetConnectionState(NWebSocket::EConnectionState::eConnectionActive);
  qDebug() << QString("Negotiation attempt to '%1' successful!").arg(fName());
    }
    else {
 qDebug() << QString("Negotiation attempt to '%1' failed!").arg(fName());
    }
  }

  void NWebSocket::fRegisterApi(const QVariantMap& lApiMap) {
    QJsonDocument lApi(QJsonDocument::fromVariant(lApiMap));
        //     QString lApi(fBuildApi().toJson(QJsonDocument::Indented));
       //***   NMessageRequest lApiRegister(QDate::currentDate(), QTime::currentTime(), mApiBuilder.fBuildApi(this));
       //****   mPacketProcessor.fProcessRequest(lApiRegister);
   //      qDebug() << "ss"; // lApiRegister.fToJsonString(QJsonDocument::Indented).toLatin1();
      //      mWebSocket.sendTextMessage(lApi);

       //   return pApiBuilder->fBuildApi(this);
  }
}
