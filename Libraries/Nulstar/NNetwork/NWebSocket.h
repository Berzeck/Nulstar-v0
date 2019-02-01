#ifndef NWEBSOCKET_H
#define NWEBSOCKET_H

#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QUrl>
#include <QWebSocket>

#include "NMessage.h"
#include "NNetwork.h"

QT_FORWARD_DECLARE_CLASS(QTimer)

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NWebSocket : public QWebSocket {
    Q_OBJECT

    public:
      enum class EConnectionState { eConnectionNotNegotiated = 1, eConnectionAuthorized = 2, eConnectionActive = 3 };

      NWebSocket(QObject* rParent = nullptr);
      NWebSocket(const QString& lName, const QString& lProtocolVersion, const QUrl& lConnectionUrl, quint8 lConnectionRetryInterval = 0, QObject* rParent = nullptr);
      virtual ~NWebSocket() override {}
      EConnectionState fConnectionState() { return mConnectionState; }
      QString fName() { return mName; }
      void fSetName(const QString& lName) { mName = lName; }
      void fSetConnectionState(const EConnectionState lConnectionState = EConnectionState::eConnectionNotNegotiated) { mConnectionState = lConnectionState; emit sStateChanged(mConnectionState); }

    public Q_SLOTS:
      void fQueueMessage(NMessage* lMessage, EConnectionState lMinStateRequired = EConnectionState::eConnectionActive);
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

    protected Q_SLOTS:
       virtual void fNegotiateConnection();
       virtual void fRegisterApi();
       virtual void fSendMessage(NMessage* rMessage);
       virtual void fOnConnectionError(QAbstractSocket::SocketError lErrorCode);
       virtual void fOnConnected();
       virtual void fOnSocketDisconnection();
       virtual void fOnTextMessageReceived(const QString& lMessage);

    Q_SIGNALS:
       void sStateChanged(EConnectionState lNewState);
  };
}

#endif // NWEBSOCKET_H
