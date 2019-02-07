//
// Created by daviyang35 on 2019-01-10.
//

#ifndef NULSTAR_NUPDATECONTROLLER_H
#define NULSTAR_NUPDATECONTROLLER_H

#include "NCoreService.h"
#include "UpdaterVersion.h"

namespace NulstarNS {
  class NUpdateController : public NCoreService {
    Q_OBJECT

    public:
      explicit NUpdateController(QWebSocketServer::SslMode lSslMode,
                             ELogLevel lLogLevel, const QHostAddress& lIP,
                             const QUrl &lServiceManagerUrl,
                             QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry>(),
                             quint16 lCommPort = 0,
                             QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null,
                             QObject *rParent = nullptr);

      ~NUpdateController() override {}
      QString fAbbreviation() const override { return "NUP"; }
      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QVariantMap fDependencies() const override {  QVariantMap lDependencies( {{cRole_ServiceManager, cVersion_ServiceManagerRole}} ); return lDependencies; }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    protected:
      void fFillMethodMetadata() override { }

    private:
      quint64 mRequestID;
      quint8 mCompressionLevel;
  };
}

#endif //NULSTAR_NUPDATECONTROLLER_H
