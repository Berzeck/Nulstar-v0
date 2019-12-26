#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QUrl>

#include "NUpdateController.h"

namespace NulstarNS {
  NUpdateController::NUpdateController(QWebSocketServer::SslMode lSslMode,
                                                NulstarNS::ELogLevel lLogLevel, const QHostAddress &lIP,
                                                const QUrl &lServiceManagerUrl, const QString &lMainPath,
                                                quint16 lCheckUpdatesInterval, const QString &lPackageSource,
                                                QList<QNetworkAddressEntry> lAllowedNetworks,
                                                quint16 lCommPort,
                                                QHostAddress::SpecialAddress lBindAddress,
                                                QObject *rParent)
                    : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lMainPath, lAllowedNetworks, rParent), mRequestID(0), mCompressionLevel(0), mCheckUpdatesInterval(lCheckUpdatesInterval),
                      mDownloadsDir(QString("%1/%2/%3").arg(fMainPath()).arg(cDirName_Downloads).arg(fCurrentOS())), mPackageSourceUrl(lPackageSource) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);

    if(mDownloadsDir.mkpath(mDownloadsDir.path())) {
      mDownloadsDir.cdUp();
      connect(&mDownloader, &NDownloader::sLog, this, &NUpdateController::sLog);
      connect(&mDownloader, &NDownloader::sFinished, this, &NUpdateController::fProcessFinishedDownload);
      connect(&mCheckUpdatesTimer, &QTimer::timeout, this, &NUpdateController::fDownloadLatestVersionsList);
      mCheckUpdatesTimer.start(mCheckUpdatesInterval * 1000);
    }
    else {
      fLog(NulstarNS::ELogLevel::eLogCritical, NulstarNS::ELogMessageType::eResourceManagement, QString("Download directory '%1' couldn't be created.").arg(mDownloadsDir.path()));
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
    if(lDownloadUrl.toString() == QString("%1/%2").arg(mPackageSourceUrl.toString()).arg(cFileName_Versions)) { // Versions.txt
      fVerifyIfNewUpdateIsAvailble(QString("%1/%2").arg(mDownloadsDir.path()).arg(cFileName_Versions));
      return;
    }

    QSettings lVersionSettings(QString("%1/%2").arg(mDownloadsDir.path()).arg(cFileName_Versions), QSettings::IniFormat);
    lVersionSettings.beginGroup("LatestVersions");
    QString lLastVersion(lVersionSettings.value(fCurrentOS()).toString());
    lVersionSettings.endGroup();
    QString lProduct = mPackageSourceUrl.path().section("/",1,1);
    QString lFullPackageName(QString("%1_%2_%3").arg(lProduct).arg(fCurrentOS()).arg(lLastVersion));
    QString lManifestUrl(QString("%1/%2/%3/%4").arg(mPackageSourceUrl.toString()).arg(fCurrentOS()).arg(lFullPackageName).arg(cFileName_VersionManifest));
    if(lDownloadUrl.toString() == lManifestUrl) { // Version.manifest
      fLoadNewVersionData(QString("%1/%2/%3").arg(mDownloadsDir.path()).arg(fCurrentOS()).arg(cFileName_VersionManifest));
      return;
    }

    else {

    }
  }

  void NUpdateController::fVerifyIfNewUpdateIsAvailble(const QUrl& lDownloadUrl) {
    QSettings lVersionSettings(lDownloadUrl.toString(), QSettings::IniFormat);
    lVersionSettings.beginGroup("LatestVersions");
    QString lLastVersion(lVersionSettings.value(fCurrentOS()).toString());
    QString lProduct = mPackageSourceUrl.path().section("/",1,1);
    lVersionSettings.endGroup();
    if(QFile::exists(QString("%1/%2").arg(fMainPath()).arg(cFileName_VersionManifest))) {
      QSettings lCurrentVersionSettings(QString("%1/%2").arg(fMainPath()).arg(cFileName_VersionManifest), QSettings::IniFormat);
      lCurrentVersionSettings.beginGroup("PackageSummary");
      QString lCurrentVersion(lCurrentVersionSettings.value(cFieldName_VersionNumber).toString());
      lCurrentVersionSettings.endGroup();
      QVersionNumber lLatestVersionNumber(QVersionNumber::fromString(lLastVersion));
      QVersionNumber lCurrentVersionNumber(QVersionNumber::fromString(lCurrentVersion));
      if(lLatestVersionNumber > lCurrentVersionNumber)
        fDownloadManifest(lLastVersion);
    }
    else {
      fDownloadManifest(lLastVersion);
    }
  }

  void NUpdateController::fLoadNewVersionData(const QString& lNewVersionManifestPath) {
    mNewVersionData.clear();
    QSettings lVersionManifest(lNewVersionManifestPath, QSettings::IniFormat);
    lVersionManifest.beginGroup("PackageSummary");
    QVariantMap lPackageSummary;
    lPackageSummary[cFieldName_VersionSoftwareName] = lVersionManifest.value(cFieldName_VersionSoftwareName);
    lPackageSummary[cFieldName_VersionPlatform] = lVersionManifest.value(cFieldName_VersionPlatform);
    lPackageSummary[cFieldName_VersionPriority] = lVersionManifest.value(cFieldName_VersionPriority);
    lPackageSummary[cFieldName_VersionReleaseDate] = lVersionManifest.value(cFieldName_VersionReleaseDate);
    lPackageSummary[cFieldName_VersionUpgradeNotes] = lVersionManifest.value(cFieldName_VersionUpgradeNotes);
    lPackageSummary[cFieldName_VersionName] = lVersionManifest.value(cFieldName_VersionName);
    lPackageSummary[cFieldName_VersionNumber] = lVersionManifest.value(cFieldName_VersionNumber);
    mNewVersionData["PackageSummary"] = lPackageSummary;
    lVersionManifest.endGroup();
  }

  void NUpdateController::fDownloadLatestVersionsList() {
    mDownloader.fDownload(QUrl(QString("%1/%2").arg(mPackageSourceUrl.toString()).arg(cFileName_Versions)), QString("%1/%2").arg(mDownloadsDir.path()).arg(cFileName_Versions));
  }

  void NUpdateController::fDownloadManifest(const QString& lVersion) {
    QString lProduct = mPackageSourceUrl.path().section("/",1,1);
    QString lFullPackageName(QString("%1_%2_%3").arg(lProduct).arg(fCurrentOS()).arg(lVersion));
    QUrl lManifestUrl(QString("%1/%2/%3/%4").arg(mPackageSourceUrl.toString()).arg(fCurrentOS()).arg(lFullPackageName).arg(cFileName_VersionManifest));
    QString lLocalPath(QString("%1/%2/%3").arg(mDownloadsDir.path()).arg(fCurrentOS()).arg(cFileName_VersionManifest));
    mDownloader.fDownload(lManifestUrl, lLocalPath);
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
