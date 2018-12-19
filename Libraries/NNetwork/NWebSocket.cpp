#include "NWebSocket.h"


namespace NulstarNS {
  NWebSocket::NWebSocket(QObject* rParent)
            : QWebSocket(QString(), QWebSocketProtocol::VersionLatest, rParent), mConnectionState(EConnectionState::eConnectionNotNegotiated) {

  }
}
