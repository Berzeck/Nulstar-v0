#include <QJsonDocument>
#include <QTimer>

#include "NWebSocket.h"
#include "NMessageNegotiateConnection.h"

namespace NulstarNS {
  NWebSocket::NWebSocket(const QString &lName, const QString &lProtocolVersion, const QUrl& lConnectionUrl, quint8 lConnectionRetryInterval, QObject* rParent)
            : QWebSocket(QString(), QWebSocketProtocol::VersionLatest, rParent), mConnectionRetryInterval(lConnectionRetryInterval), mConnectionState(EConnectionState::eConnectionNotNegotiated),
              mName(lName), mProtocolVersion(lProtocolVersion), mConnectionUrl(lConnectionUrl) {
    connect(this, &NWebSocket::connected, this, &NWebSocket::fOnConnected);
    connect(this, &NWebSocket::disconnected, this, &NWebSocket::fOnSocketDisconnection);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&NWebSocket::error), this, &NWebSocket::fOnConnectionError);
    connect(this, &NWebSocket::textMessageReceived, this, &NWebSocket::fOnTextMessageReceived);

    pRetryTimer = new QTimer(this);
    connect(pRetryTimer, &QTimer::timeout, this, &NWebSocket::fConnect);
  }

  NWebSocket::NWebSocket(QObject* rParent)
            : QWebSocket(QString(), QWebSocketProtocol::VersionLatest, rParent), pRetryTimer(nullptr) {

  }

  void NWebSocket::fConnect() {
    if(state() == QAbstractSocket::UnconnectedState && mConnectionUrl.isValid()) {
      open(mConnectionUrl);
    }
  }

  void NWebSocket::fQueueMessage(NMessage* lMessage, EConnectionState lMinStateRequired) {
    mMessageQueue.append(lMessage);
    mMessageResponses.insert(lMessage->fMessageID().toULongLong(), lMessage);
    lMessage->fSetStatus(NMessage::EMessageStatus::eAwaitingDelivery);
    if((lMinStateRequired <= fConnectionState()) && (state() == QAbstractSocket::ConnectedState))
      fSendMessage(lMessage);
  }

  void NWebSocket::fSendMessage(NMessage* rMessage) {
    qint64 lBytesSent = sendTextMessage(rMessage->fToJsonString());
    if(lBytesSent)
      rMessage->fSetStatus(NMessage::EMessageStatus::eSent);
    else
      rMessage->fSetStatus(NMessage::EMessageStatus::eWithErrorAndWitheld);
  }

  void NWebSocket::fOnConnectionError(QAbstractSocket::SocketError lErrorCode) {
    qDebug("%s", qUtf8Printable(QString::number(lErrorCode)));
    if(pRetryTimer && mConnectionRetryInterval)
      pRetryTimer->start(mConnectionRetryInterval * 1000);
  }

  void NWebSocket::fOnConnected() {
    if(fConnectionState() == NWebSocket::EConnectionState::eConnectionNotNegotiated)
      fNegotiateConnection();
    if(pRetryTimer)
      pRetryTimer->stop();
  }

  void NWebSocket::fOnSocketDisconnection() {
    fSetConnectionState(NWebSocket::EConnectionState::eConnectionNotNegotiated);
    if(pRetryTimer && mConnectionRetryInterval)
      pRetryTimer->start(mConnectionRetryInterval * 1000);
  }

  void NWebSocket::fOnTextMessageReceived(const QString& lTextMessage) {
   // mWebSocket.sendTextMessage(QString("Receieved:\n%1").arg(lTextMessage));
    qDebug("%s", qUtf8Printable(lTextMessage));
  }

  void NWebSocket::fNegotiateConnection() {
    fQueueMessage(new  NMessageNegotiateConnection(mName, QString(), mProtocolVersion, 0, NMessageNegotiateConnection::ECompressionAlgorithm::eZlib, this), EConnectionState::eConnectionNotNegotiated);
  }

  void NWebSocket::fRegisterApi() {
 //     QJsonDocument lApi(QJsonDocument::fromVariant(pApiBuilder->fBuildApi(this)));
        //     QString lApi(fBuildApi().toJson(QJsonDocument::Indented));
       //***   NMessageRequest lApiRegister(QDate::currentDate(), QTime::currentTime(), mApiBuilder.fBuildApi(this));
       //****   mPacketProcessor.fProcessRequest(lApiRegister);
   //      qDebug() << "ss"; // lApiRegister.fToJsonString(QJsonDocument::Indented).toLatin1();
      //      mWebSocket.sendTextMessage(lApi);

       //   return pApiBuilder->fBuildApi(this);
  }
}