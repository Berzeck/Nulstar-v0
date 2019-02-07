#ifndef NSERVICEMANAGERCONTROLLER_H
#define NSERVICEMANAGERCONTROLLER_H

#include <QList>
#include <QHostAddress>
#include <QObject>
#include <QVersionNumber>
#include <NCoreService.h>
#include <NWebSocketServer.h>

#include "ServiceManagerVersion.h"

namespace NulstarNS {
  class NWebSocketServer;
  class NServiceManagerController : public NCoreService {
    Q_OBJECT

    public:
      explicit NServiceManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl,  QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                                         QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NServiceManagerController() override {}

      QString fAbbreviation() const override { return "NSM"; }
      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QVariantMap fDependencies() const override {  return QVariantMap(); }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    protected:
      void fFillMethodMetadata() override { }
  };
}

#endif // NServiceManagerController_H
