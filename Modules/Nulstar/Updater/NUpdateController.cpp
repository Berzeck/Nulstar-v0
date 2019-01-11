//
// Created by daviyang35 on 2019-01-10.
//

#include "NUpdateController.h"

NulstarNS::NUpdateController::NUpdateController(QWebSocketServer::SslMode lSslMode,
                                                NulstarNS::NCoreService::ELogLevel lLogLevel,
                                                const QUrl &lServiceManagerUrl,
                                                QList<QNetworkAddressEntry> lAllowedNetworks,
                                                quint16 lCommPort,
                                                quint16 lAdminPort,
                                                quint16 lClientPort,
                                                QHostAddress::SpecialAddress lBindAddress,
                                                QObject *rParent)
    : NCoreService(lSslMode, lLogLevel, lServiceManagerUrl, lAllowedNetworks, lCommPort, lBindAddress, rParent),
      mRequestID(0), mCompressionLevel(0) {

}
