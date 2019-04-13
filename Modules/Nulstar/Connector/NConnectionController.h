
#ifndef NCONNECTIONCONTROLLER_H
#define NCONNECTIONCONTROLLER_H

#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QObject>
#include <QUrl>
#include <NCoreService.h>
#include <QVersionNumber>

#include "ConnectorVersion.h"
//***#include <NResponse.h>

namespace NulstarNS {
  const QString cAdminServerLabel = "Nulstar Management";
  const QString cAdminServerName = "WebAdminServer";
  const QString cClientServerLabel = "Nulstar Client Channel";
  const QString cClientServerName = "WebClientServer";

  class NWebSocketServer;
  class NConnectionController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl,
                                     QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                                     quint16 lAdminPort = 0, quint16 lClientPort = 0, QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NConnectionController() override {}

      QString fAbbreviation() const override { return "NCN"; }
      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QVariantMap fDependencies() const override {  QVariantMap lDependencies( {{"Role_UpdateManager", "0.1"}} ); return lDependencies; }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    protected:
      void fFillMethodMetadata() override { }
      void fFillMethodDescriptions();
      void fFillMethodMinEventAndMinPeriod();
      void fInvokeMethod(TMessageRequestToProcess& lMessageRequestToProcess) override;

    private:
      quint64 mRequestID;
      quint8 mCompressionLevel;
      QVariantMap mAdminMethods;
      QVariantMap mPublicMethods;
      QVariantMap mPrivateMethods;
      QMap<QString, TMessageRequestToProcess> mForwardedMessages; // Forwarded MessageID, Data
      void fSendBadMethodResponse(const TMessageRequestToProcess& lMessageRequestToProcess);
      void fForwardMessage(const TMessageRequestToProcess& lMessageRequestToProcess);
      bool fMethodValid(const QString& lMethodName, const QVariantMap& lMethodList);

    public Q_SLOTS:
      API_PUBLIC_FUNCTION void listapi(const TMessageRequestToProcess& lMessageRequest);
  /***    API_ADMIN_FUNCTION NResponse setmaxconnections(int lMaxConnections) { return fSetMaxConnections(lClientServerName, lMaxConnections); }
      API_ADMIN_FUNCTION NResponse getmaxconnections() { return fMaxConnections(lClientServerName); }
      API_ADMIN_FUNCTION NResponse gettotalconnections() { return fTotalConnections(lClientServerName); }
      API_ADMIN_FUNCTION NResponse setcompressionlevel(quint8 lCompressionLevel);
      API_PUBLIC_FUNCTION NResponse getcompressionlevel();  ***/
    protected Q_SLOTS:
      void fProcessResponse(const QVariantMap& lMessageResponse) override;
  };
}

#endif // NConnectionController_H
