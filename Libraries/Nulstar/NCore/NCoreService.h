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
#include <QVersionNumber>
#include <NWebSocket.h>
#include <NWebSocketServer.h>
#include "NApiBuilder.h"
#include "Core.h"

namespace NulstarNS {    
  class CORESHARED_EXPORT NCoreService : public QObject {
    Q_OBJECT

    public:
      enum class ELogLevel {eLogCritical = 1, eLogImportant = 2, eLogWarning = 3, eLogInfo = 4, eLogEverything = 5};
      enum class EServiceAction {eStartService = 0, eStopService = 1, eRestartService = 2};      

      NCoreService(QWebSocketServer::SslMode lSslMode = QWebSocketServer::SslMode::NonSecureMode, ELogLevel lLogLevel = ELogLevel::eLogWarning, const QUrl& lServiceManagerUrl = QUrl(),
                   const QList<QNetworkAddressEntry>& lAllowedNetworks = QList<QNetworkAddressEntry> (), QObject* rParent = nullptr);
      virtual ~NCoreService();

      virtual QString fName() const = 0;
      virtual QString fVersion() const = 0;
      virtual QString fDomain() const = 0;
      virtual QString fApiRole() const = 0;
      virtual QList<QVersionNumber> fApiVersionsSupported() const = 0;


      virtual bool fAddWebSocketServer(quint16 lPort, QHostAddress::SpecialAddress lBindAddress, const QString& lName = QString(), const QString& lLabel = QString(), bool lStartImmediatly = false);
      void fAddMethodFunctionDescription(const QString& lMethodName, const QString& lDescription) { mApiMethodDescription[lMethodName] = lDescription; }
      void fAddMethodMinEventAndMinPeriod(const QString& lMethodName, const QString& lApiMethodMinEventAndMinPeriod ) { mApiMethodMinEventAndMinPeriod[lMethodName] = lApiMethodMinEventAndMinPeriod; }
   /***   NMessageResponse fMaxConnections(const QString &lName);
      NResponse fTotalConnections(const QString &lName);
      NResponse fSetMaxConnections(const QString& lName, int lMaxconnections); ***/
      QString fMethodDescription(const QString& lMethodName) const;
      QString fMethodMinEventAndMinPeriod(const QString& lMethodName) const;
      void fSetSslMode(QWebSocketServer::SslMode lSslMode = QWebSocketServer::SslMode::NonSecureMode) { mSslMode = lSslMode; }
      void fSetLogLevel(ELogLevel lLogLevel = ELogLevel::eLogWarning) { mLogLevel = lLogLevel; }
      void fSetServiceManagerUrl(const QUrl& lServiceManagerUrl) { if(lServiceManagerUrl.isValid()) mServiceManagerUrl = lServiceManagerUrl; }
      void fSetAllowedNetworks(const QList<QNetworkAddressEntry>& lAllowedNetworks) { mAllowedNetworks = lAllowedNetworks; }

    protected:
      ELogLevel mLogLevel;

    private:
      quint64 mLastID;
      QUrl mServiceManagerUrl;
      NWebSocketServer::SslMode mSslMode;
      QMap<QString, NWebSocket* > mWebSockets;  // Module Name, Connection
      QMap<QString, NWebSocketServer*> mWebServers;
      QMap<QString, QString> mApiMethodDescription;
      QMap<QString, QString> mApiMethodMinEventAndMinPeriod;
      QList<QNetworkAddressEntry> mAllowedNetworks;
      NApiBuilder mApiBuilder;

    public Q_SLOTS:
      virtual void fConnectToServiceManager(quint8 lReconnectionTryInterval);
      virtual bool fControlWebServer(const QString& lName, EServiceAction lAction); // If lName is empty then it controls all web sockets servers            
  };
}

#endif // NCORESERVICE_H
