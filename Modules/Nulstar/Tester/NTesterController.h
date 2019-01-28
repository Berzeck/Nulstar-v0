
#ifndef NTESTERCONTROLLER_H
#define NTESTERCONTROLLER_H

#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QUrl>
#include <NCoreService.h>
#include <QVersionNumber>

#include "AppVersion.h"


namespace NulstarNS {
  const QString lAdminServerLabel = "Nulstar Management";
  const QString lAdminServerName = "WebAdminServer";
  const QString lClientServerLabel = "Nulstar Client Channel";
  const QString lClientServerName = "WebClientServer";

  class NWebSocketServer;
  class NTesterController : public NCoreService {
    Q_OBJECT

    public:
      explicit NTesterController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QUrl& lServiceManagerUrl,  QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                                 QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NTesterController() override {}

      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QString fApiRole() const override { return QString(APP_ROLE); }
      QList<QVersionNumber> fApiVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_ROLE_VERSION)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }
  };
}

#endif // NTesterController_H
