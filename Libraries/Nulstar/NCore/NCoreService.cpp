#include <QTimer>
#include "NModuleAPI.h"
#include "NCoreService.h"

namespace NulstarNS {
  NCoreService::NCoreService(NWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl, const QList<QNetworkAddressEntry>& lAllowedNetworks,
                             QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mServiceManagerUrl(lServiceManagerUrl), mIP(lIP), mSslMode(lSslMode), mAllowedNetworks(lAllowedNetworks) {
    QTimer::singleShot(0, this, [this] { mApiBuilder.fBuildApi(this); });
  }

  NCoreService::~NCoreService() {
    for(NWebSocketServer* rWebServer : mWebServers) {
      if(rWebServer->isListening()) rWebServer->close();
      rWebServer->deleteLater();
    }
    for(NWebSocket* rWebSocket : mWebSockets) {
      if(rWebSocket) {
        rWebSocket->fClose();
        rWebSocket->deleteLater();
      }
    }
  }

  bool NCoreService::fAddWebSocketServer(quint16 lPort, QHostAddress::SpecialAddress lBindAddress, const QString& lName, const QString& lLabel, bool lStartImmediatly) {
    if(mWebServers.contains(lName))
      return false;

    QString lEffectiveName(lName);
    QString lEffectiveLabel(lLabel);
    if(lEffectiveName.isEmpty())
      lEffectiveName = cCommServerName;
    if(lEffectiveLabel.isEmpty())
      lEffectiveLabel = cCommServerLabel;

    NWebSocketServer* pWebServer = new NWebSocketServer(lEffectiveName, lEffectiveLabel, mSslMode, fProtocolVersionsSupported(), nullptr);
    pWebServer->fSetPort(lPort);
    pWebServer->fSetBindAddress(lBindAddress);
    connect(pWebServer, &NWebSocketServer::sRequestMessageArrived, this, &NCoreService::fOnRequestMessageArrived);
    connect(pWebServer, &NWebSocketServer::sWebSocketDisconnected, this, &NCoreService::fOnWebSocketDisconnected);
    mWebServers[pWebServer->fName()] = pWebServer;
    if(lStartImmediatly)
      fControlWebServer(pWebServer->fName(), EServiceAction::eStartService);
    return true;
  }

  quint16 NCoreService::fCommPort() const {
    if(mWebServers.contains(cCommServerName))
      return mWebServers.value(cCommServerName)->fPort();
    return 0;
  }

  void NCoreService::fConnectToServiceManager(quint8 lReconnectionTryInterval) {
    NWebSocket* rWebSocket = nullptr;
    if(mWebSockets.contains(cServiceManagerName)) {
      rWebSocket = mWebSockets.value(cServiceManagerName);
      rWebSocket->fConnect();
    }
    else {
      rWebSocket = new NWebSocket(cServiceManagerName, fProtocolVersionsSupported().at(0).toString(), mServiceManagerUrl, lReconnectionTryInterval);
      connect(rWebSocket, &NWebSocket::sStateChanged, this, &NCoreService::fOnConnectionStateChanged);
      mWebSockets.insert(cServiceManagerName, rWebSocket);
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

  void NCoreService::fOnConnectionStateChanged(NWebSocket::EConnectionState lNewState) {
     NWebSocket* rWebSocket = qobject_cast<NWebSocket* > (sender());
     if(rWebSocket) {
         switch(lNewState) {
           case NWebSocket::EConnectionState::eConnectionActive: {
             rWebSocket->fRegisterApi(mApiBuilder.fApi());
             break;
           }
           default:
             break;
         }
     }
  }

  bool NCoreService::fControlWebServer(const QString &lName, EServiceAction lAction) {
    QStringList lWebServerNames;
    if(lName.isEmpty()) lWebServerNames = mWebServers.keys();
    else lWebServerNames << lName;
    for( const QString& lCurrentName : lWebServerNames) {
      if(!mWebServers.contains(lCurrentName))
        return false;
      if(lAction == EServiceAction::eStartService) {
        if(!mWebServers[lCurrentName]->fListen()) {
          qDebug() << QString("Websockets server '%1' of module '%2' could not initiated!").arg(lCurrentName).arg(fName());
          return false;
        }
      }
      if(lAction == EServiceAction::eStopService) {
        mWebServers[lCurrentName]->close();
      }
      if(lAction == EServiceAction::eRestartService) {
        mWebServers[lCurrentName]->close();
        if(!mWebServers[lCurrentName]->fListen()) {
          qDebug() << QString("Websockets server '%1' of module '%2' could not initiated!").arg(lCurrentName).arg(fName());
          return false;
        }
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
    else return cDefaultMinEventAndMinPeriod;
  }

  void NCoreService::fSendMessage(const QString& lWebSocketsServerName, NMessage& lMessage) {
    if(mWebServers.contains(lWebSocketsServerName)) {
      mWebServers.value(lWebSocketsServerName)->fSendMessage(lMessage.fConnectionName().toLongLong(), lMessage);
    }
    else {
      qDebug("%s", qUtf8Printable(QString("Message '%1' couldn't be sent because WebSocket server '%2' wasn't found!").arg(lMessage.fMessageID()).arg(lWebSocketsServerName)));
    }
  }

  void NCoreService::fOnRequestMessageArrived(const QString& lWebSocketsServerName, const QString& lWebSocketID, const QString& lMessageID, const QString& lMethodName, const QVariantMap& lParameters, qint64 lMSecsSinceEpoch) {
    QString lEffectiveMethodName(lMethodName);
    if(fApiMethodLowercase())
      lEffectiveMethodName = lMethodName.toLower();
    bool lSuccess = metaObject()->invokeMethod(this, lEffectiveMethodName.toLatin1().data(), Qt::DirectConnection, Q_ARG(QString, lWebSocketsServerName), Q_ARG(QString, lWebSocketID), Q_ARG(QString, lMessageID), Q_ARG(QVariantMap, lParameters), Q_ARG(qint64, lMSecsSinceEpoch));
    if(!lSuccess)
      qDebug("%s", qUtf8Printable(QString("Method '%1' couldn't be executed successfully!").arg(lMethodName)));
  }
}
