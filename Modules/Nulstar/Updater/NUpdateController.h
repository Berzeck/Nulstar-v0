
#ifndef NULSTAR_NUPDATECONTROLLER_H
#define NULSTAR_NUPDATECONTROLLER_H

#include <QDir>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <QVariantMap>
#include <NDownloader.h>
#include <NVersionManifest.h>

#include "NCoreService.h"
#include "UpdaterVersion.h"

namespace NulstarNS {
  const QString cDirName_Downloads(QStringLiteral("Downloads"));
  const QString cFileName_Versions(QStringLiteral("Versions.txt"));
  const QString cFileName_VersionManifest(QStringLiteral("Version.manifest"));
 /* const QString cFieldName_VersionSoftwareName(QStringLiteral("Name"));
  const QString cFieldName_VersionPlatform(QStringLiteral("Platform"));
  const QString cFieldName_VersionPriority(QStringLiteral("Priority"));
  const QString cFieldName_VersionReleaseDate(QStringLiteral("ReleaseDate"));
  const QString cFieldName_VersionUpgradeNotes(QStringLiteral("UpgradeNotes"));
  const QString cFieldName_VersionName(QStringLiteral("VersionName"));
  const QString cFieldName_VersionNumber(QStringLiteral("VersionNumber"));*/

  class NUpdateController : public NCoreService {
    Q_OBJECT

    public:
      explicit NUpdateController(QWebSocketServer::SslMode lSslMode,
                             ELogLevel lLogLevel, const QHostAddress& lIP,
                             const QUrl &lServiceManagerUrl,
                             const QString& lMainPath,
                             quint16 lCheckUpdatesInterval,
                             const QString& lPackageSource,
                             QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry>(),
                             quint16 lCommPort = 0,
                             QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null,
                             QObject *rParent = nullptr);

      ~NUpdateController() override {}
      QString fAbbreviation() const override { return "NUP"; }
      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QVariantMap fDependencies() const override {  QVariantMap lDependencies( {{"Role_ConnectionManager" ,"0.1"}} ); return lDependencies; }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    public Q_SLOTS:
      API_PUBLIC_FUNCTION void checkupdates(const TMessageRequestToProcess& lMessageRequest);
      void fProcessFinishedDownload(const QUrl& lDownloadUrl, const QByteArray& lFileContents);

    protected:
      void fFillMethodMetadata() override { }
      QString fCurrentOS() const;
      void fVerifyIfNewUpdateIsAvailble(const QUrl &lDownloadUrl);
      void fDownloadManifest(const QString& lVersion);
      void fLoadNewVersionData(const QString& lNewVersionManifestPath);

    protected Q_SLOTS:
      void fDownloadLatestVersionsList();

    private:
      quint64 mRequestID;
      quint8 mCompressionLevel;      
      quint16 mCheckUpdatesInterval;
      NDownloader mDownloader;
      QTimer mCheckUpdatesTimer;
      QDir mDownloadsDir;
      QUrl mPackageSourceUrl;
      NVersionManifest mCurrentVersionManifest;
      NVersionManifest mLatestVersionManifest;

    Q_SIGNALS:
      void sCheckUpdatesTriggered();

  };
}

#endif //NULSTAR_NUPDATECONTROLLER_H
