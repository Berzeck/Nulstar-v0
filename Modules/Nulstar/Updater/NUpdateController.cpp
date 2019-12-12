#include <QCoreApplication>
#include <QDir>
#include <QSettings>
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
                    : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent), mRequestID(0), mCompressionLevel(0), mCheckUpdatesInterval(lCheckUpdatesInterval), mPackageSourceUrl(lPackageSource) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);

     QDir lMainAppDir(qApp->applicationDirPath());
     lMainAppDir.cdUp();
     lMainAppDir.cdUp();
     lMainAppDir.cdUp();
     lMainAppDir.cdUp();
     QDir lDownloadsDir(QString("%1/%2/%3").arg(lMainAppDir.path()).arg(cDirName_Downloads).arg(fCurrentOS()));
    if(lDownloadsDir.mkpath(lDownloadsDir.path())) {
      lDownloadsDir.cdUp();
      connect(&mDownloader, &NDownloader::sLog, this, &NUpdateController::sLog);
      connect(&mDownloader, &NDownloader::sFinished, this, &NUpdateController::fProcessFinishedDownload);
      connect(&mCheckUpdatesTimer, &QTimer::timeout, this, [lDownloadsDir, this] () {
        mDownloader.fDownload(QUrl(QString("%1/%2").arg(mPackageSourceUrl).arg(cFileName_Versions)), QString("%1/%2").arg(lDownloadsDir.path()).arg(cFileName_Versions));
      } );

      mCheckUpdatesTimer.start(mCheckUpdatesInterval * 1000);
    }
    else {
      fLog(NulstarNS::ELogLevel::eLogCritical, NulstarNS::ELogMessageType::eResourceManagement, QString("Download directory '%1' couldn't be created.").arg(lDownloadsDir.path()));
    }
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

  void NUpdateController::fProcessFinishedDownload(const QUrl& lDownloadUrl, const QByteArray& /*lFileContents*/) {
    if(lDownloadUrl.toString() == QString("%1/%2").arg(mPackageSourceUrl).arg(cFileName_Versions)) { // Versions.txt
      QSettings lVersionSettings(lDownloadUrl.toString(), QSettings::IniFormat);
      lVersionSettings.beginGroup("LatestVersions");
      QString lLastVersion(lVersionSettings.value(fCurrentOS()).toString());
      lVersionSettings.endGroup();
    }
    else {

    }
  }

  QString NUpdateController::fCurrentOS() const {
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
    return lOS;
  }
}
