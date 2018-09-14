#ifndef NCORESERVICE_H
#define NCORESERVICE_H

#ifndef Q_MOC_RUN
#define API_PUBLIC_FUNCTION
#define API_PRIVATE_FUNCTION
#define API_ADMIN_FUNCTION
#endif

#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QNetworkAddressEntry>
#include <QObject>
#include <QUrl>
#include <QWebSocket>
#include <QWebSocketServer>
#include "Core.h"
#include "NResponse.h"

namespace NulstarNS {  
  class NApiBuilder;
  class NWebSocketServer;
  class CORESHARED_EXPORT NCoreService : public QObject {
    Q_OBJECT

    public:
      enum class ELogLevel {eLogCritical = 1, eLogImportant = 2, eLogWarning = 3, eLogInfo = 4, eLogEverything = 5};
      enum class EServiceAction {eStartService = 0, eStopService = 1, eRestartService = 2};

      NCoreService(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel = ELogLevel::eLogWarning, const QHostAddress& lServiceManagerIP = QHostAddress::LocalHost, QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lPort = 0, QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      virtual ~NCoreService();

      virtual void fConnectToServiceManager(const QUrl& lUrl);
      virtual bool fAddWebSocketServer(const QString& lName, const QString& lLabel, quint16 lPort, QHostAddress::SpecialAddress lBindAddress, bool lStartImmediatly = false);
      void fAddMethodFunctionDescription(const QString& lMethodName, const QString& lDescription) { mApiMethodDescription[lMethodName] = lDescription; }
      NResponse fMaxConnections(quint64 lID, QString lExternalID, const QString &lName);
      NResponse fTotalConnections(quint64 lID, QString lExternalID, const QString &lName);
      NResponse fSetMaxConnections(quint64 lID, QString lExternalID, const QString& lName, int lMaxconnections);


    protected:
      ELogLevel mLogLevel;
      virtual void fFillMethodDescriptions() { }

    private:
      QHostAddress mServiceManagerIP;
      QWebSocketServer::SslMode mSslMode;
      QWebSocket mWebSocket;
      QMap<QString, NWebSocketServer*> mWebServers;
      QMap<QString, QString> mApiMethodDescription;
      QList<QNetworkAddressEntry> mAllowedNetworks;
      NApiBuilder* pApiBuilder;

    public Q_SLOTS:
      virtual bool fControlWebServer(const QString& lName, EServiceAction lAction); // If lName is empty then it controls all web sockets servers      

    protected Q_SLOTS:
      virtual void fBuildApi();

    private Q_SLOTS:
      virtual void fOnConnected();
      virtual void fOnTextMessageReceived(const QString& lTextMessage);      

    Q_SIGNALS:
      void sClosed();
  };
}

#endif // NCORESERVICE_H
