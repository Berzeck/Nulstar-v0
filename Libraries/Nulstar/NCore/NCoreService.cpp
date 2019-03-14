#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include "NModuleAPI.h"
#include "NCoreService.h"

namespace NulstarNS {
  NCoreService::NCoreService(NWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl, const QList<QNetworkAddressEntry>& lAllowedNetworks,
                             QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mServiceManagerUrl(lServiceManagerUrl), mIP(lIP), mSslMode(lSslMode), mAllowedNetworks(lAllowedNetworks) {
    QTimer::singleShot(0, this, [this] { mApiBuilder.fBuildApi(this);
                                         QDir lLogDir(qApp->applicationDirPath());
                                         lLogDir.cdUp();lLogDir.cdUp();lLogDir.cdUp();lLogDir.cdUp();
                                         QString lLogDirString(QString("%1/%2/%3").arg(lLogDir.path()).arg(cDirectory_Logs).arg(fName()));
                                         lLogDir.mkpath(lLogDirString);
                                         pLogger = new NLogger(fName(), lLogDirString, cSeparator_Logs, mLogLevel, this);
                                         QMapIterator<QString, NWebSocketServer*> i1(mWebServers);
                                         while(i1.hasNext()) {
                                           i1.next();
                                           connect(i1.value(), &NWebSocketServer::sLog, pLogger, &NLogger::fLog);
                                         }
                                         QMapIterator<QString, NWebSocket*> i2(mWebSockets);
                                         while(i2.hasNext()) {
                                           i2.next();
                                           connect(i2.value(), &NWebSocket::sLog, pLogger, &NLogger::fLog);
                                         }
                                       });
    connect(this, &NCoreService::sEventTriggered, [this](const QString& lMethodName) {
              QMutableMapIterator<QString, TMessageRequestToProcess> i1(mMessageRequestEventQueue);
              while(i1.hasNext()) {
                i1.next();
                if((lMethodName == i1.value().mEffectiveMethodName) && ((i1.value().mEventCounter % i1.value().mSubscriptionEventCounter) == 0  )) {
                  fOnRequestMessageArrived(i1.value());
                  pLogger->fLog(ELogLevel::eLogInfo, ELogMessageType::eMemoryTransaction, QString("Message '%1' using method '%2' has been executed '%3' times.").arg(i1.key()).arg(i1.value().mEffectiveMethodName).arg(i1.value().mEventCounter));
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
    connect(pWebServer, &NWebSocketServer::sResponseMessageReceived, this, &NCoreService::fProcessResponse);
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
      pLogger->fLog(ELogLevel::eLogCritical, ELogMessageType::eMemoryTransaction,QString("WebServer '%1' has not been found when trying to remove Connection ID '%2'").arg(lWebServerName).arg(lWebSocketID));
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
      connect(rWebSocket, &NWebSocket::sMessageReceived, [this, rWebSocket](const QString& lMessageType, const QVariantMap& lMessage) {
              if(lMessageType == cTypeReponse)
                fProcessResponse(lMessage);
              if(lMessageType == cTypeRequest)
                fProcessRequest(rWebSocket->fName(), lMessage);
              });
      mWebSockets.insert(cServiceManagerName, rWebSocket);
      rWebSocket->fConnect();
    }
  }

  void NCoreService::fProcessRequest(const QString& lWebSocketName, const QVariantMap &lMessage) {

    QVariantMap lRequestMethods(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_RequestMethods).toMap());
    QString lMessageID(lMessage.value(cFieldName_MessageID).toString());
    quint64 lSubscriptionEventCounter(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionEventCounter).toULongLong());
    quint64 lSubscriptionPeriod(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionPeriod).toULongLong());
    for(const QString& lRequestMethodName : lRequestMethods.keys()) {
      QVariantMap lRequestMethodParams = lRequestMethods.value(lRequestMethodName).toMap();
      TMessageRequestToProcess lMessageRequest({lWebSocketName, lWebSocketName,lMessageID, lRequestMethodName, lRequestMethodName,lRequestMethodParams, lSubscriptionEventCounter, lSubscriptionPeriod, 0, 0} );
      fOnRequestMessageArrived(lMessageRequest);
    }
  }

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
          pLogger->fLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, QString("Websockets server '%1' of module '%2' could not initiated!").arg(lCurrentName).arg(fName()) );
          return false;
        }
      }
      if(lAction == EServiceAction::eStopService) {
        mWebServers[lCurrentName]->close();
      }
      if(lAction == EServiceAction::eRestartService) {
        mWebServers[lCurrentName]->close();
        if(!mWebServers[lCurrentName]->fListen()) {
          pLogger->fLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, QString("Websockets server '%1' of module '%2' could not initiated!").arg(lCurrentName).arg(fName()) );
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

  void NCoreService::fSendMessage(const QString& lWebSocketsID, NMessage* rMessage, NWebSocket::EConnectionState lMinStateRequired) {
    if(mWebServers.contains(lWebSocketsID)) {
      mWebServers.value(lWebSocketsID)->fSendMessage(rMessage->fConnectionName().toLongLong(), rMessage);
    }
    else {
      if(mWebSockets.contains(lWebSocketsID))
        mWebSockets.value(lWebSocketsID)->fQueueMessage(rMessage, lMinStateRequired);
      else
        pLogger->fLog(ELogLevel::eLogCritical, ELogMessageType::eMemoryTransaction, QString("Message '%1' couldn't be sent because WebSocket '%2' wasn't found!").arg(rMessage->fMessageID()).arg(lWebSocketsID));
    }
  }

  void NCoreService::fOnRequestMessageArrived(TMessageRequestToProcess& lMessageRequestToProcess) {
    lMessageRequestToProcess.mMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
    if(fApiMethodLowercase())
      lMessageRequestToProcess.mEffectiveMethodName = lMessageRequestToProcess.mEffectiveMethodName.toLower();
    fInvokeMethod(lMessageRequestToProcess);
  }  

  void NCoreService::fInvokeMethod(TMessageRequestToProcess& lMessageRequestToProcess) {
    bool lSuccess =  metaObject()->invokeMethod(this, lMessageRequestToProcess.mEffectiveMethodName.toLatin1().data(), Qt::DirectConnection, Q_ARG(TMessageRequestToProcess, lMessageRequestToProcess));
    if(lSuccess) {
      if(lMessageRequestToProcess.mSubscriptionEventCounter) {
        lMessageRequestToProcess.mEventCounter += 1;
        if(!mMessageRequestEventQueue.contains(lMessageRequestToProcess.mMessageID))
          mMessageRequestEventQueue.insert(lMessageRequestToProcess.mMessageID, lMessageRequestToProcess);
      }
    }
    else
      pLogger->fLog(ELogLevel::eLogCritical, ELogMessageType::eMemoryTransaction, QString("Method '%1' couldn't be executed successfully!").arg(lMessageRequestToProcess.mEffectiveMethodName));
  }
}
