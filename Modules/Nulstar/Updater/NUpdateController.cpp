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
      lApiRolesMap[lRole] = lSupportedVersions;
    }
    return lApiRolesMap;
  }

  void NUpdateController::checkupdates(const TMessageRequestToProcess& lMessageRequest) {
     qDebug("%s", qUtf8Printable(QString("Updater-%1").arg(lMessageRequest.mMessageID)));
     QVariantMap lGetUpdatesResponse { {"checkupdates", QVariantMap({{ "Update1", "Version1" }, { "Update2", "Version2" } }) } };
     qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
     NMessageResponse* rGetUpdatesResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseSuccessful, QString(), 0, lGetUpdatesResponse);
     fSendMessage(lMessageRequest.mWebSocketsServerName, rGetUpdatesResponse);
  }
}
