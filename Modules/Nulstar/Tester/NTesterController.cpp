#include <QHostAddress>
#include <QSettings>
#include <QWebSocket>
#include <NWebSocketServer.h>
#include <NApiBuilder.h>
#include "NTesterController.h"

namespace NulstarNS {
    NTesterController::NTesterController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QUrl &lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                        : NCoreService(lSslMode, lLogLevel, lServiceManagerUrl, lAllowedNetworks, rParent) {
        if(lCommPort)
          fAddWebSocketServer(lCommPort, lBindAddress);
    }

}

