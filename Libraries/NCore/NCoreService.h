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
#include <NWebSocket.h>
#include <NWebSocketServer.h>
#include "NApiBuilder.h"
#include "Core.h"

namespace NulstarNS {    
  const QString lConstantsFile("Constants.ncf");

  class NWebSocketServer;
  class CORESHARED_EXPORT NCoreService : public QObject {
    Q_OBJECT

    public:
      enum class ELogLevel {eLogCritical = 1, eLogImportant = 2, eLogWarning = 3, eLogInfo = 4, eLogEverything = 5};
      enum class EServiceAction {eStartService = 0, eStopService = 1, eRestartService = 2};      

      NCoreService(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel = ELogLevel::eLogWarning, const QUrl& lServiceManagerUrl = QUrl(), QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lPort = 0, QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      virtual ~NCoreService();

      virtual QString fName() const = 0;
      virtual QString fVersion() const = 0;
      virtual QString fDomain() const = 0;
      virtual QString fApiRole() const = 0;
      virtual QString fApiVersion() const = 0;

      virtual void fConnectToServiceManager();
      virtual bool fAddWebSocketServer(const QString& lName, const QString& lLabel, quint16 lPort, QHostAddress::SpecialAddress lBindAddress, bool lStartImmediatly = false);
      void fAddMethodFunctionDescription(const QString& lMethodName, const QString& lDescription) { mApiMethodDescription[lMethodName] = lDescription; }
      void fAddMethodMinEventAndMinPeriod(const QString& lMethodName, const QString& lApiMethodMinEventAndMinPeriod ) { mApiMethodMinEventAndMinPeriod[lMethodName] = lApiMethodMinEventAndMinPeriod; }
   /***   NMessageResponse fMaxConnections(const QString &lName);
      NResponse fTotalConnections(const QString &lName);
      NResponse fSetMaxConnections(const QString& lName, int lMaxconnections); ***/
      QString fMethodDescription(const QString& lMethodName) const;
      QString fMethodMinEventAndMinPeriod(const QString& lMethodName) const;

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
      virtual bool fControlWebServer(const QString& lName, EServiceAction lAction); // If lName is empty then it controls all web sockets servers      
  };
}

#endif // NCORESERVICE_H
