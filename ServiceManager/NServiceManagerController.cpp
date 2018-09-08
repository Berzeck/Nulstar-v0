#include <QHostAddress>
#include <QWebSocket>
#include "NServiceManagerController.h"

namespace NulstarNS {
  NServiceManagerController::NServiceManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                           : NCoreService(lSslMode, lLogLevel, lAllowedNetworks, lCommPort, lBindAddress, rParent) {

  }
}
