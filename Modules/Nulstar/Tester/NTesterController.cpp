#include <QHostAddress>
#include <QSettings>
#include <QWebSocket>
#include <NWebSocketServer.h>
#include <NApiBuilder.h>
#include "NTesterController.h"

namespace NulstarNS {
    NTesterController::NTesterController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress &lIP, const QUrl &lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                     : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent) {
        if(lCommPort)
          fAddWebSocketServer(lCommPort, lBindAddress);
    }

    QVariantMap NTesterController::fApiRoles() const {
      QVariantMap lApiRolesMap;
      QStringList lApiRoles(QString(APP_ROLES).split(":"));
      for(QString& lApiRole : lApiRoles) {
        lApiRole.remove(' ');
        QString lRole = lApiRole.split("[").at(0);
        QStringList lSupportedVersions = lApiRole.split("[").at(1).split("]").at(0).split(",");
        lApiRolesMap[lApiRole] = lSupportedVersions;
      }
      return lApiRolesMap;
    }
}

