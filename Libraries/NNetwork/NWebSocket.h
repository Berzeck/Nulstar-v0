#ifndef NWEBSOCKET_H
#define NWEBSOCKET_H

#include <QWebSocket>

#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NWebSocket : public QWebSocket {
    Q_OBJECT

    public:
      enum class EConnectionState { eConnectionNotNegotiated = 1, eConnectionAuthorized = 2, eConnectionActive = 3 };

      NWebSocket(QObject* rParent = nullptr);
      virtual ~NWebSocket() override {}
      EConnectionState fConnectionState() { return mConnectionState; }
      void fSetConnectionState(const EConnectionState lConnectionState = EConnectionState::eConnectionNotNegotiated) { mConnectionState = lConnectionState; }

    private:
      EConnectionState mConnectionState;
  };
}

#endif // NWEBSOCKET_H
