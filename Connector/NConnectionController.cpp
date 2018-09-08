#include <QHostAddress>
#include <QSettings>
#include <QWebSocket>
#include <NWebSocketServer.h>
#include <NApiBuilder.h>
#include "NConnectionController.h"

const QString lAdminServerLabel = "Nulstar Management";
const QString lAdminServerName = "WebAdminServer";
const QString lClientServerLabel = "Nulstar Client Channel";
const QString lClientServerName = "WebClientServer";

namespace NulstarNS {
  NConnectionController::NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, quint16 lAdminPort, quint16 lClientPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                       : NCoreService(lSslMode, lLogLevel, lAllowedNetworks, lCommPort, lBindAddress, rParent) {
    fAddWebSocketServer(lAdminServerName, lAdminServerLabel, lAdminPort, QHostAddress::Any, false);
    fAddWebSocketServer(lClientServerName, lClientServerLabel, lClientPort, QHostAddress::Any, false);
  }
}

