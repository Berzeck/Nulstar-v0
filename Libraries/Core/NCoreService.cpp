#include <QTimer>
#include <NWebSocketServer.h>
#include "NCoreService.h"
#include "NApiBuilder.h"

const QString lCommServerLabel = "Nulstar Internal Communication";
const QString lCommServerName = "WebCommServer";

namespace NulstarNS {
  NCoreService::NCoreService(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lServiceManagerIP, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lPort, QHostAddress::SpecialAddress lBindAddress, QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mServiceManagerIP(lServiceManagerIP), mSslMode(lSslMode), mAllowedNetworks(lAllowedNetworks), pApiBuilder(new NApiBuilder(this)) {

    if(mServiceManagerIP.isNull()) mServiceManagerIP = QHostAddress::LocalHost;

    fAddWebSocketServer(lCommServerName, lCommServerLabel, lPort, lBindAddress, false);
    fFillMethodDescriptions();
// We use a timer so the function fBuildApi is executed when the application enters the main loop, therefor after the derived classes are created
    QTimer::singleShot(0, this, &NCoreService::fBuildApi);
  }
  NCoreService::~NCoreService() {
    for(NWebSocketServer* rWebServer : mWebServers)
      if(rWebServer->isListening()) rWebServer->close();
  }

  bool NCoreService::fAddWebSocketServer(const QString& lName, const QString& lLabel, quint16 lPort, QHostAddress::SpecialAddress lBindAddress, bool lStartImmediatly) {
    if(mWebServers.contains(lName)) return false;

    NWebSocketServer* pWebServer = new NWebSocketServer(lName, lLabel, mSslMode, this);
    pWebServer->fSetPort(lPort);
    pWebServer->fSetBindAddress(lBindAddress);

    mWebServers[pWebServer->fName()] = pWebServer;
    if(lStartImmediatly) fControlWebServer(pWebServer->fName(), EServiceAction::eStartService);
    return true;
  }

  void NCoreService::fConnectToServiceManager(const QUrl& lUrl) {
    connect(&mWebSocket, &QWebSocket::connected, this, &NCoreService::fOnConnected);
    connect(&mWebSocket, &QWebSocket::disconnected, this, &NCoreService::sClosed);
    mWebSocket.open(lUrl);
  }

  NResponse NCoreService::fSetMaxConnections(quint64 lID, QString lExternalID, const QString& lName, int lMaxconnections) {
    if(mWebServers.contains(lName)) {
      mWebServers[lName]->fSetMaxConnections(lMaxconnections);
      NResponse lResponse(lID, lExternalID, true, true);
      return lResponse;
    }
    NResponse lResponse(lID, lExternalID, false, false, tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  }

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

  NResponse NCoreService::fMaxConnections(quint64 lID, QString lExternalID, const QString &lName) {
    if(mWebServers.contains(lName)) {
      NResponse lResponse(lID, lExternalID, true, mWebServers[lName]->fMaxConnections());
      return lResponse;
    }
    NResponse lResponse(lID, lExternalID, false, 0, tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  }

  NResponse NCoreService::fTotalConnections(quint64 lID, QString lExternalID, const QString &lName) {
    if(mWebServers.contains(lName)) {
      NResponse lResponse(lID, lExternalID, true, mWebServers[lName]->fTotalConnections());
      return lResponse;
    }
    NResponse lResponse(lID, lExternalID, false, 0, tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  }
  void NCoreService::fOnConnected() {
    connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &NCoreService::fOnTextMessageReceived);
    mWebSocket.sendTextMessage(QStringLiteral("Sending Roles!"));
setProperty("CU", QString("111"));
  }

  void NCoreService::fOnTextMessageReceived(const QString &lTextMessage) {
    mWebSocket.sendTextMessage(QString("Receieved:\n%1").arg(lTextMessage));
  }

  void NCoreService::fBuildApi() {
    pApiBuilder->fSetTargetObject(this);
    pApiBuilder->fBuildApi();
  }
}
