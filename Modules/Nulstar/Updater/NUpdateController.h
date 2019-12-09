
#ifndef NULSTAR_NUPDATECONTROLLER_H
#define NULSTAR_NUPDATECONTROLLER_H

#include <QTimer>
#include <QUrl>
#include <NDownloader.h>

#include "NCoreService.h"
#include "UpdaterVersion.h"

namespace NulstarNS {
  class NUpdateController : public NCoreService {
    Q_OBJECT

    public:
      explicit NUpdateController(QWebSocketServer::SslMode lSslMode,
                             ELogLevel lLogLevel, const QHostAddress& lIP,
                             const QUrl &lServiceManagerUrl,
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
      void fCheckUpdates();
      void fProcessFinishedDownload(const QUrl& lDownloadUrl);

    protected:
      void fFillMethodMetadata() override { }

    private:
      quint64 mRequestID;
      quint8 mCompressionLevel;      
      quint16 mCheckUpdatesInterval;
      NDownloader mDownloader;
      QTimer mCheckUpdatesTimer;
      QUrl mPackageSourceUrl;
  };
}

#endif //NULSTAR_NUPDATECONTROLLER_H
