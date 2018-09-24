#include <QTimer>
#include <NWebSocketServer.h>
#include "NCoreService.h"
#include "NApiBuilder.h"

#include <QDebug>

const QString lCommServerLabel = "Nulstar Internal Communication";
const QString lCommServerName = "WebCommServer";

namespace NulstarNS {
  NCoreService::NCoreService(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QUrl& lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lPort, QHostAddress::SpecialAddress lBindAddress, QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mServiceManagerUrl(lServiceManagerUrl), mSslMode(lSslMode), mAllowedNetworks(lAllowedNetworks), pApiBuilder(new NApiBuilder(this)) {
    fAddWebSocketServer(lCommServerName, lCommServerLabel, lPort, lBindAddress, false);
    fFillMethodDescriptions();
    QTimer::singleShot(500, this, &NCoreService::fConnectToServiceManager);
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

  void NCoreService::fConnectToServiceManager() {
    if(mServiceManagerUrl.isValid()) {
      connect(&mWebSocket, &QWebSocket::connected, this, &NCoreService::fOnConnected, Qt::UniqueConnection);
      connect(&mWebSocket, &QWebSocket::disconnected, this, &NCoreService::sClosed, Qt::UniqueConnection);
      connect(&mWebSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &NCoreService::fOnConnectionError);
      mWebSocket.open(mServiceManagerUrl);
    }
  }

  NResponse NCoreService::fSetMaxConnections(const QString& lName, int lMaxconnections) {
    if(mWebServers.contains(lName)) {
      mWebServers[lName]->fSetMaxConnections(lMaxconnections);
      NResponse lResponse(true, true);
      return lResponse;
    }
    NResponse lResponse(false, false, tr("Web server '%1' not found.").arg(lName));
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

  NResponse NCoreService::fMaxConnections(const QString &lName) {
    if(mWebServers.contains(lName)) {
      NResponse lResponse(true, mWebServers[lName]->fMaxConnections());
      return lResponse;
    }
    NResponse lResponse(false, 0, tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  }

  NResponse NCoreService::fTotalConnections(const QString &lName) {
    if(mWebServers.contains(lName)) {
      NResponse lResponse(true, mWebServers[lName]->fTotalConnections());
      return lResponse;
    }
    NResponse lResponse(false, 0, tr("Web server '%1' not found.").arg(lName));
    return lResponse;
  }

  void NCoreService::fOnConnectionError(QAbstractSocket::SocketError lErrorCode) {
    qDebug(QString::number(lErrorCode).toLatin1());
    qDebug("ERRORR!!");
    qDebug(mWebSocket.errorString().toLatin1());
  }

  void NCoreService::fOnConnected() {
    connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &NCoreService::fOnTextMessageReceived, Qt::UniqueConnection);
    QString lApi(fBuildApi().toJson(QJsonDocument::Indented));
  qDebug(lApi.toLatin1());  
    mWebSocket.sendTextMessage(lApi);
  }

  void NCoreService::fOnTextMessageReceived(const QString &lTextMessage) {
   // mWebSocket.sendTextMessage(QString("Receieved:\n%1").arg(lTextMessage));
qDebug(lTextMessage.toLatin1());
  }

  QJsonDocument NCoreService::fBuildApi() {
    pApiBuilder->fSetTargetObject(this);
    QJsonDocument lApi(QJsonDocument::fromVariant(pApiBuilder->fBuildApi()));
    return lApi;
  }
}
