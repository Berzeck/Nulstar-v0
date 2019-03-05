#ifndef NWEBSOCKET_H
#define NWEBSOCKET_H

#include <QByteArray>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include <QWebSocket>
#include <QSettings>

#include "NMessage.h"
#include "NNetwork.h"

QT_FORWARD_DECLARE_CLASS(QTimer)

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NWebSocket : public QObject {
    Q_OBJECT

    public:
      enum class EConnectionState { eConnectionNotNegotiated = 1, eConnectionAuthorized = 2, eConnectionActive = 3 };

      NWebSocket(const QString& lName, const QString& lProtocolVersion, const QUrl& lConnectionUrl, quint8 lConnectionRetryInterval = 0, QWebSocket* rSocket = nullptr, QObject* rParent = nullptr);
      virtual ~NWebSocket() {}


      qint64 fSendTextMessage(const QString& lMessage) { return mWebSocket->sendTextMessage(lMessage); }
      EConnectionState fConnectionState() { return mConnectionState; }
      QString fName() { return mName; }
      void fSetName(const QString& lName) { mName = lName; }
      void fSetConnectionState(const EConnectionState lConnectionState = EConnectionState::eConnectionNotNegotiated) { mConnectionState = lConnectionState; emit sStateChanged(mConnectionState); }

    public Q_SLOTS:
      void fRegisterApi(const QVariantMap& lApiMap);
      void fQueueMessage(NMessage* rMessage, EConnectionState lMinStateRequired = EConnectionState::eConnectionActive);
      void fSendMessage(NMessage* rMessage);
      void fClose() { mWebSocket->close(); }
      void fConnect();

    protected:     
      void fProcessNegotiateConnectionResponse(const QJsonObject& lObjectMessage);

    private:
      quint8 mConnectionRetryInterval;
      EConnectionState mConnectionState;
      QList<NMessage*> mMessageQueue;
      QMap<quint64, NMessage* > mMessageResponses; // MessageID, Message Pointer
      QString mName;
      QString mProtocolVersion;
      QTimer* pRetryTimer;
      QUrl mConnectionUrl;
      QWebSocket* mWebSocket;
      QSettings* mLog;

    protected Q_SLOTS:
       virtual void fNegotiateConnection();              
       virtual void fOnConnectionError(QAbstractSocket::SocketError lErrorCode);
       virtual void fOnConnected();
       virtual void fOnSocketDisconnection();
       virtual void fOnTextMessageReceived(const QString& lMessage);

    Q_SIGNALS:
       void sConnected();
       void sDisconnected();
       void sError(QAbstractSocket::SocketError lErrorCode);
       void sStateChanged(EConnectionState lNewState);
       void sMessageReceived(const QString& lMessageType, const QVariantMap& lMessage);
  };
}

#endif // NWEBSOCKET_H
