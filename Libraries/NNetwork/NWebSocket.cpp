#include <QJsonDocument>
#include "NWebSocket.h"
#include "NMessageNegotiateConnection.h"

namespace NulstarNS {
  NWebSocket::NWebSocket(const QString &lModuleName, QObject* rParent)
            : QWebSocket(QString(), QWebSocketProtocol::VersionLatest, rParent), mConnectionState(EConnectionState::eConnectionNotNegotiated), mModuleName(lModuleName) {
    connect(this, &NWebSocket::connected, this, &NWebSocket::fOnConnected);
    connect(this, &NWebSocket::disconnected, this, &NWebSocket::fOnSocketDisconnection);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&NWebSocket::error), this, &NWebSocket::fOnConnectionError);
    connect(this, &NWebSocket::textMessageReceived, this, &NWebSocket::fOnTextMessageReceived, Qt::UniqueConnection);

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
  //  qDebug() << "ERRORR!!";
   // qDebug() << errorString();
  }

  void NWebSocket::fOnConnected() {
    if(fConnectionState() == NWebSocket::EConnectionState::eConnectionNotNegotiated)
      fNegotiateConnection();
  }

  void NWebSocket::fOnSocketDisconnection() {
    fSetConnectionState(NWebSocket::EConnectionState::eConnectionNotNegotiated);
  }

  void NWebSocket::fOnTextMessageReceived(const QString& lTextMessage) {
   // mWebSocket.sendTextMessage(QString("Receieved:\n%1").arg(lTextMessage));
    qDebug("%s", qUtf8Printable(lTextMessage));
  }

  void NWebSocket::fNegotiateConnection() {
    fQueueMessage(new  NMessageNegotiateConnection(mModuleName, QString(), 0, NMessageNegotiateConnection::ECompressionAlgorithm::eZlib, this), EConnectionState::eConnectionNotNegotiated);
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
