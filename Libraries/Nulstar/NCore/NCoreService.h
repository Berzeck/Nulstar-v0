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
#include <QString>
#include <QUrl>
#include <QVariantMap>
#include <QVersionNumber>
#include <NMessage.h>
#include <NMessageRequest.h>
#include <NMessageResponse.h>
#include <NWebSocket.h>
#include <NWebSocketServer.h>
#include "NApiBuilder.h"
#include "Core.h"
#include "NCoreConstants.h"

namespace NulstarNS {    
  class CORESHARED_EXPORT NCoreService : public QObject {
    Q_OBJECT

    public:
      enum class ELogLevel {eLogCritical = 1, eLogImportant = 2, eLogWarning = 3, eLogInfo = 4, eLogEverything = 5};
      enum class EServiceAction {eStartService = 0, eStopService = 1, eRestartService = 2};      

      NCoreService(QWebSocketServer::SslMode lSslMode = QWebSocketServer::SslMode::NonSecureMode, ELogLevel lLogLevel = ELogLevel::eLogWarning, const QHostAddress& lIP = QHostAddress::SpecialAddress::LocalHost,
                   const QUrl& lServiceManagerUrl = QUrl(), const QList<QNetworkAddressEntry>& lAllowedNetworks = QList<QNetworkAddressEntry> (), QObject* rParent = nullptr);
      virtual ~NCoreService();

      virtual QString fAbbreviation() const = 0;
      virtual QString fName() const = 0;
      virtual QString fVersion() const = 0;
      virtual QString fDomain() const = 0;
      virtual QVariantMap fDependencies() const = 0;
      virtual QVariantMap fApiRoles() const = 0;
      virtual QHostAddress fHostAddress() const { return mIP; }
      virtual QList<QVersionNumber> fProtocolVersionsSupported() const = 0;
      virtual quint16 fCommPort() const;
    //  virtual int fApiMethodNameOffset() const { return 1; }
      virtual bool fApiMethodLowercase() const { return true; }

      virtual bool fAddWebSocketServer(quint16 lPort, QHostAddress::SpecialAddress lBindAddress, const QString& lName = QString(), const QString& lLabel = QString(), bool lStartImmediatly = false);
      void fAddMethodFunctionDescription(const QString& lMethodName, const QString& lDescription) { mApiMethodDescription[lMethodName] = lDescription; }
      void fAddMethodMinEventAndMinPeriod(const QString& lMethodName, const QString& lApiMethodMinEventAndMinPeriod ) { mApiMethodMinEventAndMinPeriod[lMethodName] = lApiMethodMinEventAndMinPeriod; }
   /***   NMessageResponse fMaxConnections(const QString &lName);
      NResponse fTotalConnections(const QString &lName);
      NResponse fSetMaxConnections(const QString& lName, int lMaxconnections); ***/
      QString fMethodDescription(const QString& lMethodName) const;
      QString fMethodMinEventAndMinPeriod(const QString& lMethodName) const;
      QUrl fServiceManagerUrl() const { return mServiceManagerUrl; }
      void fSetSslMode(QWebSocketServer::SslMode lSslMode = QWebSocketServer::SslMode::NonSecureMode) { mSslMode = lSslMode; }
      void fSetLogLevel(ELogLevel lLogLevel = ELogLevel::eLogWarning) { mLogLevel = lLogLevel; }
      void fSetServiceManagerUrl(const QUrl& lServiceManagerUrl) { if(lServiceManagerUrl.isValid()) mServiceManagerUrl = lServiceManagerUrl; }
      void fSetAllowedNetworks(const QList<QNetworkAddressEntry>& lAllowedNetworks) { mAllowedNetworks = lAllowedNetworks; }
      void fSetHost(const QHostAddress& lIP) { mIP = lIP; }
      void fSendMessage(const QString& lWebSocketsServerName, NMessage* rMessage);

    public Q_SLOTS:
      virtual void fConnectToServiceManager(quint8 lReconnectionTryInterval);
      virtual void fOnConnectionStateChanged(NWebSocket::EConnectionState lNewState);
      virtual bool fControlWebServer(const QString& lName, EServiceAction lAction); // If lName is empty then it controls all web sockets servers
      virtual void fCloseConnection(const QString& lWebServerName, const QString& lWebSocketID);

    Q_SIGNALS:
      void sEventTriggered(const QString& lMethodName);

    protected Q_SLOTS:
      virtual void fOnRequestMessageArrived(TMessageRequestToProcess& lMessageRequestToProcess);
      virtual void fOnWebSocketDisconnected(const QString& lWebSocketID) { Q_UNUSED(lWebSocketID); }  

    protected:
      virtual void fFillMethodMetadata() = 0;
      ELogLevel mLogLevel;

    private:
      quint64 mLastID;
      QUrl mServiceManagerUrl;
      QHostAddress mIP;
      NWebSocketServer::SslMode mSslMode;
      QMap<QString, TMessageRequestToProcess > mMessageRequestEventQueue; // MessageID, Struct RequestToProcess
      QMap<QString, NWebSocket* > mWebSockets;  // Module Name, Connection
      QMap<QString, NWebSocketServer*> mWebServers;
      QMap<QString, QString> mApiMethodDescription;
      QMap<QString, QString> mApiMethodMinEventAndMinPeriod;
      QList<QNetworkAddressEntry> mAllowedNetworks;
      NApiBuilder mApiBuilder;
  };
}

#endif // NCORESERVICE_H
