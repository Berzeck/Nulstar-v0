#ifndef NCONNECTIONCONTROLLER_H
#define NCONNECTIONCONTROLLER_H

#include <QHostAddress>
#include <QObject>
#include <QUrl>
#include <NCoreService.h>
#include <NResponse.h>

namespace NulstarNS {
  const QString lAdminServerLabel = "Nulstar Management";
  const QString lAdminServerName = "WebAdminServer";
  const QString lClientServerLabel = "Nulstar Client Channel";
  const QString lClientServerName = "WebClientServer";

  class NWebSocketServer;
  class NConnectionController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QUrl& lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                                     quint16 lAdminPort = 0, quint16 lClientPort = 0, QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NConnectionController() override {}

      QString fName() override { return QString(APP_NAME).replace("_"," "); }
      QString fVersion() override { return QString(APP_VERSION); }
      QString fDomain() override { return QString(APP_DOMAIN); }
      QString fApiRole() override { return QString(APP_ROLE); }
      QString fApiVersion() override { return QString(APP_ROLE_VERSION); }

    protected:
      void fFillMethodDescriptions() override;

    private:
      quint64 mRequestID;
      quint8 mCompressionLevel;

    public Q_SLOTS:
      API_ADMIN_FUNCTION NResponse setmaxconnections(quint64 lID, QString lExternalID, int lMaxConnections) { return fSetMaxConnections(lID, lExternalID, lClientServerName, lMaxConnections); }
      API_ADMIN_FUNCTION NResponse getmaxconnections(quint64 lID, QString lExternalID) { return fMaxConnections(lID, lExternalID,lClientServerName); }
      API_ADMIN_FUNCTION NResponse gettotalconnections(quint64 lID, QString lExternalID) { return fTotalConnections(lID, lExternalID, lClientServerName); }
      API_ADMIN_FUNCTION NResponse setcompressionlevel(quint64 lID, QString lExternalID, quint8 lCompressionLevel);
      API_PUBLIC_FUNCTION NResponse getcompressionlevel(quint64 lID, QString lExternalID);

  };
}

#endif // NConnectionController_H
