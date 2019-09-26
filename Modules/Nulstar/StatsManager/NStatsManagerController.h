#ifndef NSTATSMANAGERCONTROLLER_H
#define NSTATSMANAGERCONTROLLER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QVersionNumber>

#include <NCoreService.h>
#include "StatsManagerVersion.h"

namespace NulstarNS {
  class NStatsManagerController : public NCoreService {
    Q_OBJECT

    public:
      explicit NStatsManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl,  QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                                       QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NStatsManagerController() override {}

      QString fAbbreviation() const override { return "NSTM"; }
      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QVariantMap fDependencies() const override {  QVariantMap lDependencies( { } ); return lDependencies; }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    public Q_SLOTS:
      API_ADMIN_FUNCTION void GetCPUInfo();
      API_ADMIN_FUNCTION void GetRAMInfo();

    protected:
       void fFillMethodMetadata() override;
  };
}

#endif // NStatsManagerController_H
