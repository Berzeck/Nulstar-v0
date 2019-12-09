//
// Created by daviyang35 on 2019-01-10.
//

#include <QUrl>

#include "NUpdateController.h"

namespace NulstarNS {
  NUpdateController::NUpdateController(QWebSocketServer::SslMode lSslMode,
                                                NulstarNS::ELogLevel lLogLevel, const QHostAddress &lIP,
                                                const QUrl &lServiceManagerUrl,
                                                quint16 lCheckUpdatesInterval, const QString &lPackageSource,
                                                QList<QNetworkAddressEntry> lAllowedNetworks,
                                                quint16 lCommPort,
                                                QHostAddress::SpecialAddress lBindAddress,
                                                QObject *rParent)
                    : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent), mRequestID(0), mCompressionLevel(0), mCheckUpdatesInterval(lCheckUpdatesInterval) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);
    connect(&mDownloader, &NDownloader::sLog, this, &NUpdateController::sLog);
    connect(&mDownloader, &NDownloader::sFinished, this, &NUpdateController::fProcessFinishedDownload);
    connect(&mCheckUpdatesTimer, &QTimer::timeout, this, &NUpdateController::fCheckUpdates);
    mCheckUpdatesTimer.start(mCheckUpdatesInterval * 1000);

    QString lOS;
#ifdef Q_OS_WIN64
    lOS = QStringLiteral("Windows");
#endif
#ifdef Q_OS_LINUX
    lOS = QStringLiteral("Linux");
#endif
#ifdef Q_OS_MACOS
    lOS = QStringLiteral("MacOS");
#endif

    mPackageSourceUrl = QUrl(QString("%1/%2/").arg(lPackageSource).arg(lOS));
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
    NMessageResponse* rGetUpdatesResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseSuccessful, QString(), 0, lGetUpdatesResponse, QString());
    fSendMessage(lMessageRequest.mWebSocketsServerName, rGetUpdatesResponse);
  }

  void NUpdateController::fCheckUpdates() {
    mDownloader.fDownload(QUrl("http://drive.nulstar.com/Nuls/Linux/Nuls_Linux_2.2.1/Nuls_Linux_2.2.1.manifest"), "/home/Berzeck/Templates/aaa/Nuls_Linux_2.2.1.manifest");
    qDebug("%s", qUtf8Printable(mPackageSourceUrl.toString()));
  }

  void NUpdateController::fProcessFinishedDownload(const QUrl& lDownloadUrl) {

  }
}
