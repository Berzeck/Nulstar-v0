#ifndef NTESTERCONTROLLER_H
#define NTESTERCONTROLLER_H

#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QUrl>
#include <NCoreService.h>
#include <QVersionNumber>

#include "TesterVersion.h"


namespace NulstarNS {
  const QString lAdminServerLabel = "Nulstar Management";
  const QString lAdminServerName = "WebAdminServer";
  const QString lClientServerLabel = "Nulstar Client Channel";
  const QString lClientServerName = "WebClientServer";

  class NWebSocketServer;
  class NTesterController : public NCoreService {
    Q_OBJECT

    public:
      explicit NTesterController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl, const QString& lMainPath, QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                                 QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NTesterController() override {}

      QString fAbbreviation() const override { return "NTS"; }
      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QVariantMap fDependencies() const override {  QVariantMap lDependencies({ }); return lDependencies; }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    protected:
      void fFillMethodMetadata() override { }
  };
}

#endif // NTesterController_H
