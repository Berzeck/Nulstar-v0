#include <QTimer>
#include "NCoreService.h"

namespace NulstarNS {
  const QString lCommServerLabel("Nulstar Internal Communication");
  const QString lCommServerName("WebCommServer");
  const QString lDefaultMinEventAndMinPeriod("0,0");
  const QString lServiceManagerName("ServiceManager") ;

  NCoreService::NCoreService(NWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QUrl& lServiceManagerUrl, const QList<QNetworkAddressEntry>& lAllowedNetworks,
                             quint16 lPort, QHostAddress::SpecialAddress lBindAddress, QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mServiceManagerUrl(lServiceManagerUrl), mSslMode(lSslMode), mAllowedNetworks(lAllowedNetworks) {
    if(lPort)
      fAddWebSocketServer(lPort, lBindAddress, lCommServerName, lCommServerLabel, false);
    QTimer::singleShot(0, this, [this] { mApiBuilder.fBuildApi(this); });
  }

  NCoreService::~NCoreService() {
    for(NWebSocketServer* rWebServer : mWebServers) {
      if(rWebServer->isListening()) rWebServer->close();
      rWebServer->deleteLater();
    }
    for(NWebSocket* rWebSocket : mWebSockets) {
      if(rWebSocket->isValid()) rWebSocket->close(QWebSocketProtocol::CloseCodeGoingAway, tr("Shutting Down"));
      rWebSocket->deleteLater();
    }
  }

  bool NCoreService::fAddWebSocketServer(quint16 lPort, QHostAddress::SpecialAddress lBindAddress, const QString& lName, const QString& lLabel, bool lStartImmediatly) {
    if(mWebServers.contains(lName))
      return false;

    QString lEffectiveName(lName);
    QString lEffectiveLabel(lLabel);
    if(lEffectiveName.isEmpty())
      lEffectiveName = lCommServerName;
    if(lEffectiveLabel.isEmpty())
      lEffectiveLabel = lCommServerLabel;

    NWebSocketServer* pWebServer = new NWebSocketServer(lName, lLabel, mSslMode, nullptr);
    pWebServer->fSetPort(lPort);
    pWebServer->fSetBindAddress(lBindAddress);

    mWebServers[pWebServer->fName()] = pWebServer;
    if(lStartImmediatly)
      fControlWebServer(pWebServer->fName(), EServiceAction::eStartService);
    return true;
  }

  void NCoreService::fConnectToServiceManager(quint8 lReconnectionTryInterval) {
    NWebSocket* rWebSocket = nullptr;
    if(mWebSockets.contains(lServiceManagerName)) {
      rWebSocket = mWebSockets.value(lServiceManagerName);
      rWebSocket->fConnect();
    }
    else {
      rWebSocket = new NWebSocket(lServiceManagerName, fApiVersion(), mServiceManagerUrl, lReconnectionTryInterval);
      mWebSockets.insert(lServiceManagerName, rWebSocket);
      rWebSocket->fConnect();
    }
  }

  /*** NResponse NCoreService::fSetMaxConnections(const QString& lName, int lMaxconnections) {
    if(mWebServers.contains(lName)) {
      mWebServers[lName]->fSetMaxConnections(lMaxconnections);
      NResponse lResponse(true, true);
      return lResponse;
    }
    NResponse lResponse(false, false, QDate::currentDate().toString("yyyy-MM-dd"), QTime::currentTime().toString("hh:mm:ss"), tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  } ***/

  bool NCoreService::fControlWebServer(const QString &lName, EServiceAction lAction) {
    QStringList lWebServerNames;
    if(lName.isEmpty()) lWebServerNames = mWebServers.keys();
    else lWebServerNames << lName;
    for( const QString& lCurrentName : lWebServerNames) {
      if(!mWebServers.contains(lCurrentName)) return false;
      if(lAction == EServiceAction::eStartService) {
        mWebServers[lCurrentName]->fListen();
      }
      if(lAction == EServiceAction::eStopService) {
        mWebServers[lCurrentName]->close();
      }
      if(lAction == EServiceAction::eRestartService) {
        mWebServers[lCurrentName]->close();
        mWebServers[lCurrentName]->fListen();
      }
    }
    return true;
  }

 /*** NResponse NCoreService::fMaxConnections(const QString &lName) {
    if(mWebServers.contains(lName)) {
      NResponse lResponse(true, mWebServers[lName]->fMaxConnections());
      return lResponse;
    }
    NResponse lResponse(false, 0, tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  }***/

  QString NCoreService::fMethodDescription(const QString& lMethodName) const {
    return mApiMethodDescription.value(lMethodName);
  }

  QString NCoreService::fMethodMinEventAndMinPeriod(const QString& lMethodName) const {
    if(mApiMethodMinEventAndMinPeriod.contains(lMethodName)) return mApiMethodMinEventAndMinPeriod.value(lMethodName);
    else return lDefaultMinEventAndMinPeriod;
  }

/***  NResponse NCoreService::fTotalConnections(const QString &lName) {
    if(mWebServers.contains(lName)) {
      NResponse lResponse(true, mWebServers[lName]->fTotalConnections());
      return lResponse;
    }
    NResponse lResponse(false, 0, tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  } ***/
}
