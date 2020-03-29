
#ifndef NULSTAR_NUPDATECONTROLLER_H
#define NULSTAR_NUPDATECONTROLLER_H

#include <QDir>
#include <QString>
#include <QStringList>
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
  const QString cFieldName_UpdateAvailable(QStringLiteral("UpdateAvailable"));

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
      API_ADMIN_FUNCTION void updatesystem(const TMessageRequestToProcess& lMessageRequest);
      void fProcessFinishedDownload(const QString &lDownloadID, const QByteArray& lFileContents);

    protected:
      void fFillMethodMetadata() override { }
      QString fCurrentOS() const;
      void fVerifyIfNewUpdateIsAvailble(const QUrl& lLocalDownloadUrl);
      void fDownloadManifest(const QString& lVersion);
      void fDownloadPackages();
      QStringList fGenerateDownloadList() const;

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
