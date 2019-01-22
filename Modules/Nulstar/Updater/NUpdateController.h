//
// Created by daviyang35 on 2019-01-10.
//

#ifndef NULSTAR_NUPDATECONTROLLER_H
#define NULSTAR_NUPDATECONTROLLER_H
#pragma once
#include "NCoreService.h"
#include "AppVersion.h"

namespace NulstarNS {

class NUpdateController : public NCoreService {
public:
  explicit NUpdateController(QWebSocketServer::SslMode lSslMode,
                             ELogLevel lLogLevel,
                             const QUrl &lServiceManagerUrl,
                             QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry>(),
                             quint16 lCommPort = 0,
                             quint16 lAdminPort = 0,
                             quint16 lClientPort = 0,
                             QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null,
                             QObject *rParent = nullptr);

  ~NUpdateController() override {}
  QString fName() const override { return QString(APP_NAME); }
  QString fVersion() const override { return QString(APP_VERSION); }
  QString fDomain() const override { return QString(APP_DOMAIN); }
  QString fApiRole() const override { return QString(APP_ROLE); }
  QList<QVersionNumber> fApiVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_ROLE_VERSION)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

private:
  quint64 mRequestID;
  quint8 mCompressionLevel;

};

}
#endif //NULSTAR_NUPDATECONTROLLER_H
