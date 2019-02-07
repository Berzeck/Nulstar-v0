//
// Created by daviyang35 on 2019-01-10.
//

#include "NUpdateController.h"

namespace NulstarNS {
  NUpdateController::NUpdateController(QWebSocketServer::SslMode lSslMode,
                                                NulstarNS::NCoreService::ELogLevel lLogLevel, const QHostAddress &lIP,
                                                const QUrl &lServiceManagerUrl,
                                                QList<QNetworkAddressEntry> lAllowedNetworks,
                                                quint16 lCommPort,
                                                QHostAddress::SpecialAddress lBindAddress,
                                                QObject *rParent)
                    : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent), mRequestID(0), mCompressionLevel(0) {

     if(lCommPort)
       fAddWebSocketServer(lCommPort, lBindAddress);
  }

  QVariantMap NUpdateController::fApiRoles() const {
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
