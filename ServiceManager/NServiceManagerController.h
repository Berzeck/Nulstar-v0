#ifndef NSERVICEMANAGERCONTROLLER_H
#define NSERVICEMANAGERCONTROLLER_H

#include <QHostAddress>
#include <QObject>
#include <NCoreService.h>
#include <NWebSocketServer.h>

#include "../AppVersion.h"

namespace NulstarNS {
  class NWebSocketServer;
  class NServiceManagerController : public NCoreService {
    Q_OBJECT

    public:
      explicit NServiceManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QUrl& lServiceManagerUrl,  QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                                         QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NServiceManagerController() override {}

      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QString fApiRole() const override { return QString(APP_ROLE); }
      QString fApiVersion() const override { return QString(APP_ROLE_VERSION); }
  };
}

#endif // NServiceManagerController_H
