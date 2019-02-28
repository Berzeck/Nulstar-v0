#include <QTimer>
#include "NModuleAPI.h"
#include "NCoreService.h"

namespace NulstarNS {
  NCoreService::NCoreService(NWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl, const QList<QNetworkAddressEntry>& lAllowedNetworks,
                             QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mServiceManagerUrl(lServiceManagerUrl), mIP(lIP), mSslMode(lSslMode), mAllowedNetworks(lAllowedNetworks) {
    QTimer::singleShot(0, this, [this] { mApiBuilder.fBuildApi(this); });
    connect(this, &NCoreService::sEventTriggered, [this](const QString& lMethodName) {
              QMutableMapIterator<QString, TMessageRequestToProcess> i1(mMessageRequestEventQueue);
              while(i1.hasNext()) {
                i1.next();
                if((lMethodName == i1.value().mMethodName) && ((i1.value().mEventCounter % i1.value().mSubscriptionEventCounter) == 0  )) {
                  fOnRequestMessageArrived(i1.value());
                  qDebug("%s", qUtf8Printable(QString("Message '%1' using method '%2' has been executed '%3' times.").arg(i1.key()).arg(i1.value().mMethodName).arg(i1.value().mEventCounter)));
                }
              }
            } );
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

  void NCoreService::fCloseConnection(const QString& lWebServerName, const QString& lWebSocketID) {
    if(mWebServers.contains(lWebServerName)) {
      mWebServers.value(lWebServerName)->fRemoveConnections(QList<qint64>() << lWebSocketID.toLongLong());
    }
    else {
      qDebug("%s", qUtf8Printable(QString("WebServer '%1' has not been found when trying to remove Connection ID '%2'").arg(lWebServerName).arg(lWebSocketID)));
    }
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

  void NCoreService::fSendMessage(const QString& lWebSocketsServerName, NMessage* rMessage) {
    if(mWebServers.contains(lWebSocketsServerName)) {
      mWebServers.value(lWebSocketsServerName)->fSendMessage(rMessage->fConnectionName().toLongLong(), rMessage);
    }
    else {
      qDebug("%s", qUtf8Printable(QString("Message '%1' couldn't be sent because WebSocket server '%2' wasn't found!").arg(rMessage->fMessageID()).arg(lWebSocketsServerName)));
    }
  }

  void NCoreService::fOnRequestMessageArrived(TMessageRequestToProcess& lMessageRequestToProcess) {
    lMessageRequestToProcess.mMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
    if(fApiMethodLowercase())
      lMessageRequestToProcess.mMethodName = lMessageRequestToProcess.mMethodName.toLower();
    bool lSuccess = metaObject()->invokeMethod(this, lMessageRequestToProcess.mMethodName.toLatin1().data(), Qt::DirectConnection, Q_ARG(TMessageRequestToProcess, lMessageRequestToProcess));
      if(lSuccess) {
        if(lMessageRequestToProcess.mSubscriptionEventCounter) {
          lMessageRequestToProcess.mEventCounter += 1;
          if(!mMessageRequestEventQueue.contains(lMessageRequestToProcess.mMessageID))
            mMessageRequestEventQueue.insert(lMessageRequestToProcess.mMessageID, lMessageRequestToProcess);
        }
      }
      else
        qDebug("%s", qUtf8Printable(QString("Method '%1' couldn't be executed successfully!").arg(lMessageRequestToProcess.mMethodName)));
  }  
}
