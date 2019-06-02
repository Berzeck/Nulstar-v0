#ifndef NSERVICEMANAGERCONTROLLER_H
#define NSERVICEMANAGERCONTROLLER_H

#include <QList>
#include <QHostAddress>
//#include <QMutex>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QVariantMap>
#include <QVersionNumber>
#include <NCoreService.h>
#include <NModuleAPI.h>
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
      QVariantMap fDependencies() const override {  QVariantMap lDependencies({ }); return lDependencies; }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    public Q_SLOTS:
      API_PRIVATE_FUNCTION void registerapi(const TMessageRequestToProcess& lMessageRequest);
      API_PRIVATE_FUNCTION void getconsolidatedapi(const TMessageRequestToProcess& lMessageRequest);
      API_PRIVATE_FUNCTION void forwardmessage(const TMessageRequestToProcess& lMessageRequest);

    protected Q_SLOTS:
      void fProcessResponse(const QVariantMap& lMessageResponse) override;
      void fOnWebSocketDisconnected(const QString& lWebSocketID) override;
      void fFindDependencies();

    protected:
      QTimer mFindDependenciesRetryTimer;
      QList<NModuleAPI> mModuleAPIPendingDependencies;
      QMap<QString, NModuleAPI> mModuleAPIActive;
      void fFillMethodMetadata() override { }

    private:
      QMap<QString, TMessageRequestToProcess> mForwardedMessages; // Forwarded MessageID, Data
  };
}

#endif // NServiceManagerController_H
