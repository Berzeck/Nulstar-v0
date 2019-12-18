#include <QCoreApplication>
#include <QDir>
#include <QMetaEnum>
#include <QMetaObject>
#include <QTimer>
#include <NRunGuard.h>
#include "NModuleAPI.h"
#include "NCoreService.h"

namespace NulstarNS {
  NCoreService::NCoreService(NWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl& lServiceManagerUrl, const QString &lMainPath, const QList<QNetworkAddressEntry>& lAllowedNetworks,
                             QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mFirstInstance(true), mMainPath(lMainPath), pRunGuard(nullptr), mServiceManagerUrl(lServiceManagerUrl), mIP(lIP), mSslMode(lSslMode), mAllowedNetworks(lAllowedNetworks) {

    pRunGuard = new NRunGuard(QString("%1%2").arg(NulstarNS::cSharedMemoryKey).arg(qApp->applicationDirPath()));
    if(!pRunGuard->fTryToRun()) {
      mFirstInstance = false;
      fLog(NulstarNS::ELogLevel::eLogCritical, NulstarNS::ELogMessageType::eMemoryTransaction, QString("Module '%1' couldn't be started because its already running!").arg(qAppName()));
      QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
    }
    if(mFirstInstance) {
      QTimer::singleShot(0, this, [this] {
                                         mApiBuilder.fBuildApi(this);
                                         QDir lLogDir(qApp->applicationDirPath());
                                         lLogDir.cdUp();lLogDir.cdUp();lLogDir.cdUp();lLogDir.cdUp();
                                         QString lLogDirString(QString("%1/%2/%3").arg(lLogDir.path()).arg(cDirectory_Logs).arg(fName()));
                                         lLogDir.mkpath(lLogDirString);
                                         pLogger = new NLogger(fName(), lLogDirString, cSeparator_Logs, mLogLevel, this);
                                         connect(this, &NCoreService::sLog, pLogger, &NLogger::fLog);
                                     /*    pRunGuard = new NRunGuard(QString("%1%2").arg(NulstarNS::cSharedMemoryKey).arg(qApp->applicationDirPath()));//
                                         bool lOneInstance = true;
                                         if(!pRunGuard->fTryToRun()) {
                                           lOneInstance = false;
                                           fLog(NulstarNS::ELogLevel::eLogCritical, NulstarNS::ELogMessageType::eMemoryTransaction, QString("Module '%1' couldn't be started because its already running!").arg(fName()));
                                           QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
                                         }*/
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
                  fLog(ELogLevel::eLogInfo, ELogMessageType::eMemoryTransaction, QString("Message '%1' using method '%2' has been executed '%3' times.").arg(i1.key()).arg(i1.value().mEffectiveMethodName).arg(i1.value().mEventCounter));
                }
              }
            } );
    }
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
    if(pRunGuard)
      delete pRunGuard;
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
      fLog(ELogLevel::eLogCritical, ELogMessageType::eMemoryTransaction,QString("WebServer '%1' has not been found when trying to remove Connection ID '%2'").arg(lWebServerName).arg(lWebSocketID));
    }
  }

  quint16 NCoreService::fCommPort() const {
    if(mWebServers.contains(cCommServerName))
      return mWebServers.value(cCommServerName)->fPort();
    return 0;
  }

  void NCoreService::fConnectToServiceManager(quint8 lReconnectionTryInterval) {
    if(!mFirstInstance)
      return;
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
                fProcessBaseResponse(lMessage);
              if(lMessageType == cTypeRequest)
                fProcessRequest(rWebSocket->fName(), lMessage);
              });
      mWebSockets.insert(cServiceManagerName, rWebSocket);
      rWebSocket->fConnect();
    }
  }

  void NCoreService::fConnectToModule(const NConnectionInfo& lConnectionInfo) {
    NWebSocket* rWebSocket = nullptr;
    quint8 lReconnectionTryInterval(lConnectionInfo.fGetReconnectionTryInterval());
    QString lRole(lConnectionInfo.fGetRole());
    QString lPort(lConnectionInfo.fGetPort());
    QString lIP(lConnectionInfo.fGetIP());
    QString lAPIVersion(lConnectionInfo.fGetRole());

    if(!fDependencies().contains(lRole)) {
      fLog(ELogLevel::eLogWarning, ELogMessageType::eResourceManagement, tr("Role '%1' not found in dependencies!").arg(lRole));
      return;
    }

    if(mWebSockets.contains(lRole)) {
      rWebSocket = mWebSockets.value(lRole);
      rWebSocket->fConnect();
    }
    else {
      QString lUrl(QString("ws://%1:%2").arg(lIP).arg(lPort));
      if(mSslMode == NWebSocketServer::SslMode::SecureMode)
        lUrl = QString("ws://%1:%2").arg(lIP).arg(lPort);

      rWebSocket = new NWebSocket(lRole, fDependencies().value(lRole).toString(), lUrl, lReconnectionTryInterval);
      connect(rWebSocket, &NWebSocket::sStateChanged, this, &NCoreService::fOnConnectionStateChanged);
      connect(rWebSocket, &NWebSocket::sMessageReceived, [this](const QString& lMessageType, const QVariantMap& lMessage) {
        if(lMessageType == cTypeReponse)
          fProcessBaseResponse(lMessage);
        else {
          fLog(ELogLevel::eLogWarning, ELogMessageType::eMessageReceived, tr("Incoming message with MessageID: '%1' has unexpected Message Type: '%2'").arg(lMessage.value(cFieldName_MessageID).toString()));
        }
      });
      mWebSockets.insert(lRole, rWebSocket);
      rWebSocket->fConnect();
    }
  }

  void NCoreService::fProcessRequest(const QString& lWebSocketName, const QVariantMap &lMessage) {
    QVariantMap lRequestMethods(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_RequestMethods).toMap());
    QString lMessageID(lMessage.value(cFieldName_MessageID).toString());
    quint64 lSubscriptionEventCounter(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionEventCounter).toULongLong());
    quint64 lSubscriptionPeriod(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionPeriod).toULongLong());
    QString lTimeOut(lMessage.value(cFieldName_MessageData).toMap().value(cFieldName_TimeOut).toString());
    for(const QString& lRequestMethodName : lRequestMethods.keys()) {
      QVariantMap lRequestMethodParams = lRequestMethods.value(lRequestMethodName).toMap();
      TMessageRequestToProcess lMessageRequest({lWebSocketName, lWebSocketName,lMessageID, lRequestMethodName, lRequestMethodName,lRequestMethodParams, lSubscriptionEventCounter, lSubscriptionPeriod, 0, 0, lTimeOut} );
      fOnRequestMessageArrived(lMessageRequest);
      return; //-- Temporal
    }
  }

  void NCoreService::fOnConnectionStateChanged(NWebSocket::EConnectionState lNewState) {
    NWebSocket* rWebSocket = qobject_cast<NWebSocket* > (sender());
    if(rWebSocket) {
      switch(lNewState) {
        case NWebSocket::EConnectionState::eConnectionActive: {
          if(rWebSocket->fName() == cServiceManagerName)
            rWebSocket->fRegisterApi(mApiBuilder.fApi());
          break;
        }
        default:
          break;
      }
      QMetaEnum lConnectionState(QMetaEnum::fromType<NWebSocket::EConnectionState>());
      fLog(ELogLevel::eLogInfo, ELogMessageType::eResourceManagement, tr("Connection '%1' changed to state '%2'.").arg(rWebSocket->fName()).arg(lConnectionState.valueToKey(static_cast<int>(lNewState))));
    }
  }

  void NCoreService::fProcessBaseResponse(const QVariantMap& lMessageResponse) {  
    int lResponseStatus(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseStatusFieldName).toInt());
    QString lResponseMessageID(lMessageResponse.value(cFieldName_MessageID).toString());
    QString lRequestMessageID(lMessageResponse.value(cFieldName_MessageData).toMap().value(cRequestIDFieldName).toString());
    if(NMessageResponse::EResponseStatus(lResponseStatus) == NMessageResponse::EResponseStatus::eResponseSuccessful) {
      if(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseDataFieldName).toMap().contains(cFieldName_RegisterAPI)) {
        QVariantMap lDependencies(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseDataFieldName).toMap().value(cFieldName_RegisterAPI).toMap()
                                  .value(cFieldName_Dependencies).toMap());
        QMapIterator<QString, QVariant> i1(lDependencies);
        while(i1.hasNext()) {
          i1.next();
          QString lRole(i1.key());
          QString lIP, lPort, lAPIVersion;
          QMapIterator<QString, QVariant> i2(i1.value().toMap());
          while(i2.hasNext()) {
            i2.next();
            if(i2.key() == cFieldName_IP)
              lIP = i2.value().toString();
            if(i2.key() == cFieldName_Port)
              lPort = i2.value().toString();
            if(i2.key() == cFieldName_ModuleRoleVersion)
              lAPIVersion = i2.value().toString();
          }
          NConnectionInfo lConnectionInfo(cRetryInterval, lRole, lIP, lPort, lAPIVersion);
          if(lConnectionInfo.fIsValid())
            fConnectToModule(lConnectionInfo);
          else
            fLog(ELogLevel::eLogWarning, ELogMessageType::eMessageReceived, tr("Connection info of dependency not valid from RegisterAPI request, RequestID: '%1'. Role: '%2'.").arg(lRequestMessageID).arg(lRole));
        }
      }
    }
    else {
      fLog(ELogLevel::eLogWarning, ELogMessageType::eMessageReceived, tr("Error returned from request '%1'. Response ID: '%2'.").arg(lRequestMessageID).arg(lResponseMessageID));
    }
    fProcessResponse(lMessageResponse);
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
          fLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, QString("Websockets server '%1' of module '%2' could not initiated!").arg(lCurrentName).arg(fName()) );
          return false;
        }
      }
      if(lAction == EServiceAction::eStopService) {
        mWebServers[lCurrentName]->close();
      }
      if(lAction == EServiceAction::eRestartService) {
        mWebServers[lCurrentName]->close();
        if(!mWebServers[lCurrentName]->fListen()) {
          fLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, QString("Websockets server '%1' of module '%2' could not initiated!").arg(lCurrentName).arg(fName()) );
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
      else {
        fLog(ELogLevel::eLogCritical, ELogMessageType::eMemoryTransaction, QString("Message '%1' couldn't be sent because WebSocket '%2' wasn't found!").arg(rMessage->fMessageID()).arg(lWebSocketsID));
        rMessage->deleteLater();
      }
    }
  }

  void NCoreService::fOnRequestMessageArrived(TMessageRequestToProcess& lMessageRequestToProcess) {
    lMessageRequestToProcess.mMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
    if(fApiMethodLowercase())
      lMessageRequestToProcess.mEffectiveMethodName = lMessageRequestToProcess.mEffectiveMethodName.toLower();
    fInvokeMethod(lMessageRequestToProcess);
  }  

  void NCoreService::fInvokeMethod(TMessageRequestToProcess& lMessageRequestToProcess) {
    bool lValidEventCounter = fValidEventCounter(lMessageRequestToProcess);
    bool lValidPeriod = fValidPeriod(lMessageRequestToProcess);
    if(!lValidEventCounter || !lValidPeriod) {
      QVariantMap lResponseMap({ {lMessageRequestToProcess.mEffectiveMethodName, QVariantMap({}) }} );
      qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequestToProcess.mMSecsSinceEpoch);
      QString lErrorMessage = !lValidEventCounter ? tr("SubscriptionEventCounter parameter: '%1' not valid for method '%2'").arg(lMessageRequestToProcess.mSubscriptionEventCounter).arg(lMessageRequestToProcess.mEffectiveMethodName) :
                                                    tr("SubscriptionPeriod parameter: '%1' not valid for method '%2'").arg(lMessageRequestToProcess.mSubscriptionPeriod).arg(lMessageRequestToProcess.mEffectiveMethodName);
      NMessageResponse* lInvalidParameterResponse = new NMessageResponse(lMessageRequestToProcess.mWebSocketID, QString(), lMessageRequestToProcess.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseMethodExeError,
                                                  lErrorMessage, 0, lResponseMap, QString("%1-%2").arg(fAbbreviation()).arg(int(NMessageResponse::EResponseStatus::eResponseMethodExeError)));
      fSendMessage(lMessageRequestToProcess.mWebSocketsServerName, lInvalidParameterResponse);
      fLog(ELogLevel::eLogCritical, ELogMessageType::eMemoryTransaction, lErrorMessage);
      return;
    }

    bool lSuccess = true;
    lSuccess =  metaObject()->invokeMethod(this, lMessageRequestToProcess.mEffectiveMethodName.toLatin1().data(), Qt::DirectConnection, Q_ARG(TMessageRequestToProcess, lMessageRequestToProcess));
    if(lSuccess) {
      if(lMessageRequestToProcess.mSubscriptionEventCounter) {
        lMessageRequestToProcess.mEventCounter += 1;
        if(!mMessageRequestEventQueue.contains(lMessageRequestToProcess.mMessageID))
          mMessageRequestEventQueue.insert(lMessageRequestToProcess.mMessageID, lMessageRequestToProcess);
      }
    }
    else {
      QVariantMap lResponseMap({ {lMessageRequestToProcess.mEffectiveMethodName, QVariantMap({}) }} );
      qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequestToProcess.mMSecsSinceEpoch);
      NMessageResponse* lMethodNotFoundResponse = new NMessageResponse(lMessageRequestToProcess.mWebSocketID, QString(), lMessageRequestToProcess.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseMethodExeError,
                                                  tr("Method '%1' couldn't be executed successfully!").arg(lMessageRequestToProcess.mEffectiveMethodName), 0, lResponseMap, QString("%1-%2").arg(fAbbreviation()).arg(int(NMessageResponse::EResponseStatus::eResponseMethodExeError)));
      fSendMessage(lMessageRequestToProcess.mWebSocketsServerName, lMethodNotFoundResponse);
      fLog(ELogLevel::eLogCritical, ELogMessageType::eMemoryTransaction, QString("Method '%1' couldn't be executed successfully!").arg(lMessageRequestToProcess.mEffectiveMethodName));
    }
  }

  bool NCoreService::fValidEventCounter(const TMessageRequestToProcess& lMessageRequestToProcess) const {
    quint64 lMinEventCounter = 0;
    if(mApiMethodMinEventAndMinPeriod.contains(lMessageRequestToProcess.mEffectiveMethodName))
      lMinEventCounter = mApiMethodMinEventAndMinPeriod[lMessageRequestToProcess.mEffectiveMethodName].section(",",0,0).toULongLong();

    if((lMinEventCounter == 0) && (lMessageRequestToProcess.mSubscriptionEventCounter > 0))
      return false;
    if((lMinEventCounter > 0) && (lMessageRequestToProcess.mSubscriptionEventCounter < lMinEventCounter))
      return false;

    return true;
  }

  bool NCoreService::fValidPeriod(const TMessageRequestToProcess& lMessageRequestToProcess) const {
    quint64 lMinEventPeriod = 0;
    if(mApiMethodMinEventAndMinPeriod.contains(lMessageRequestToProcess.mEffectiveMethodName))
      lMinEventPeriod = mApiMethodMinEventAndMinPeriod[lMessageRequestToProcess.mEffectiveMethodName].section(",",1,1).toULongLong();

    if((lMinEventPeriod == 0) && (lMessageRequestToProcess.mSubscriptionPeriod > 0))
      return false;
    if((lMinEventPeriod > 0) && (lMessageRequestToProcess.mSubscriptionPeriod < lMinEventPeriod))
      return false;

    return true;
  }
}
