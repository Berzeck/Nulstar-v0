#ifndef NWEBSOCKET_H
#define NWEBSOCKET_H

#include <QList>
#include <QMap>
#include <QWebSocket>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NWebSocket : public QWebSocket {
    Q_OBJECT

    public:
      enum class EConnectionState { eConnectionNotNegotiated = 1, eConnectionAuthorized = 2, eConnectionActive = 3 };

      NWebSocket(const QString& lModuleName, QObject* rParent = nullptr);
      virtual ~NWebSocket() override {}
      EConnectionState fConnectionState() { return mConnectionState; }
      QString fModuleName() { return mModuleName; }
      void fSetModuleName(const QString& lModuleName) { mModuleName = lModuleName; }
      void fSetConnectionState(const EConnectionState lConnectionState = EConnectionState::eConnectionNotNegotiated) { mConnectionState = lConnectionState; }

    public Q_SLOTS:
      void fQueueMessage(NMessage* lMessage, EConnectionState lMinStateRequired = EConnectionState::eConnectionActive);
 //     void fProcessQueue();

    private:
      EConnectionState mConnectionState;
      QList<NMessage*> mMessageQueue;
      QMap<quint64, NMessage* > mMessageResponses; // MessageID, Message Pointer
      QString mModuleName;

    protected Q_SLOTS:
       virtual void fNegotiateConnection();
       virtual void fRegisterApi();
       virtual void fSendMessage(NMessage* rMessage);
       virtual void fOnConnectionError(QAbstractSocket::SocketError lErrorCode);
       virtual void fOnConnected();
       virtual void fOnSocketDisconnection();
       virtual void fOnTextMessageReceived(const QString& lTextMessage);
  };
}

#endif // NWEBSOCKET_H
