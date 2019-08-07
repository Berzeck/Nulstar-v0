#include <QEventLoop>
#include <QHostAddress>
#include <qhttpserverrequest.h>
#include <QSettings>
#include <QStringList>
#include <QTimer>
#include <QWebSocket>
#include <NMessage.h>
#include <NMessageRequest.h>
#include <NWebSocketServer.h>
#include <NApiBuilder.h>

#include "NConnectionController.h"

namespace NulstarNS {
  NConnectionController::NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl &lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, quint16 lAdminPort, quint16 lClientPort,
                                               QHostAddress::SpecialAddress lBindAddress, const QString &lHttpServerPort, QObject* rParent)
                       : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent), mRequestID(0), mCompressionLevel(0), mHttpServerPort(lHttpServerPort) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);
    fAddWebSocketServer(lAdminPort, QHostAddress::AnyIPv4, cAdminServerName, cAdminServerLabel, false);
    fAddWebSocketServer(lClientPort, QHostAddress::AnyIPv4, cClientServerName, cClientServerLabel, false);
    fFillMethodDescriptions();
    fFillMethodMinEventAndMinPeriod();

    if(mHttpServerPort.toInt() > 0) {
      mHttpServer.route("/", [this](const QHttpServerRequest &lRequest) {
        QString lResponse;
        QEventLoop lTimeout;  // So components have enough time to open the websocket servers
        QTimer lTimer;
        connect(&lTimer, &QTimer::timeout, &lTimeout, &QEventLoop::quit);
        connect(this, &NConnectionController::sHttpResponseProcessed, [&lResponse, &lTimeout](const QString& lHttpResponse) {
          lResponse = lHttpResponse;
          lTimeout.quit();
        });
        fOnTextMessageReceived(lRequest.body());
        if(lResponse.isEmpty()) {
          lTimer.start(cHttpServerTimeout * 1000);
          lTimeout.exec();
        }

        if(lResponse == cHttpError) {
 qDebug("Message error!");
          QString lErrorMessage(fProcessHttpRequestError());
          return QString(lErrorMessage);
        }
        if(lResponse.isEmpty()) {
 qDebug("Message timeout!");
          fLog(NulstarNS::ELogLevel::eLogWarning, NulstarNS::ELogMessageType::eMemoryTransaction, QString("HTTP Server was unable to process the request!'"));
          QString lTimeOutMessage(fProcessHttpRequestTimeout(lRequest.body()));
          return QString(lTimeOutMessage);
        }
        return QString(lResponse);
      });

      if(mHttpServer.listen(QHostAddress::AnyIPv4, mHttpServerPort.toUShort()) <= 0) {
        fLog(NulstarNS::ELogLevel::eLogCritical, NulstarNS::ELogMessageType::eMemoryTransaction, QString("HTTP Server was unable to listen to port '%1'").arg(mHttpServerPort));
        qDebug("HTTP Server was unable to listen to port '%s'", mHttpServerPort.toStdString().data());
      }
    }
  }

  QString NConnectionController::fProcessHttpRequestTimeout(const QString& lMessage) {
    QString lMessageType;
    QJsonObject lMessageObject(NMessageFactory::fMessageObjectFromString(lMessage, &lMessageType));
    QVariantMap lMessageMap(lMessageObject.toVariantMap());
    QVariantMap lRequestMethods(lMessageMap.value(cFieldName_MessageData).toMap().value(cFieldName_RequestMethods).toMap());
    QString lMessageID(lMessageMap.value(cFieldName_MessageID).toString());
    quint64 lSubscriptionEventCounter(lMessageMap.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionEventCounter).toULongLong());
    quint64 lSubscriptionPeriod(lMessageMap.value(cFieldName_MessageData).toMap().value(cFieldName_SubscriptionPeriod).toULongLong());
    for(const QString& lRequestMethodName : lRequestMethods.keys()) {
      QVariantMap lRequestMethodParams = lRequestMethods.value(lRequestMethodName).toMap();
      TMessageRequestToProcess lMessageRequest({cHttpServerName, cHttpServerName,lMessageID, lRequestMethodName, lRequestMethodName,lRequestMethodParams, lSubscriptionEventCounter, lSubscriptionPeriod, 0, 0} );
      lMessageRequest.mMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
      if(fApiMethodLowercase())
        lMessageRequest.mEffectiveMethodName = lMessageRequest.mEffectiveMethodName.toLower();

      qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
      NMessageResponse* rRequestResponse = new NMessageResponse(cHttpServerName, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseMethodUnavailableError,
                                           tr("Method '%1' timeout!").arg(lRequestMethodName), 0, QVariantMap({{lMessageRequest.mOriginalMethodName, QVariantMap()}} ), QString("%1-%2").arg(fAbbreviation()).arg(int(NMessageResponse::EResponseStatus::eResponseMethodUnavailableError)));
      return rRequestResponse->fToJsonString();
    }
  }

  QString NConnectionController::fProcessHttpRequestError() {
    TMessageRequestToProcess lMessageRequest({cHttpServerName, cHttpServerName,"0", "", "", QVariantMap(), 0, 0, 0, 0} );
    lMessageRequest.mMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
    qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
    NMessageResponse* rRequestResponse = new NMessageResponse(cHttpServerName, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseMessageFormatError,
                                         tr("Message is not in JSon format!"), 0, QVariantMap({{lMessageRequest.mOriginalMethodName, QVariantMap()}} ), QString("%1-%2").arg(fAbbreviation()).arg(int(NMessageResponse::EResponseStatus::eResponseMessageFormatError)));
    return rRequestResponse->fToJsonString();
  }

  void NConnectionController::fOnTextMessageReceived(const QString& lMessage) {
    //qDebug() << "\nConnector: Text Message received:" << lMessage;
    QString lMessageType;
    QJsonObject lMessageObject(NMessageFactory::fMessageObjectFromString(lMessage, &lMessageType));
    if(lMessageType == cTypeRequest && NMessageRequest::fValidateMessageObject(lMessageObject)) {
       fProcessRequest(cHttpServerName, lMessageObject.toVariantMap());
       emit sLog(ELogLevel::eLogInfo, ELogMessageType::eMessageReceived, lMessage);
    }
    else {
      emit sLog(ELogLevel::eLogCritical, ELogMessageType::eMessageReceived, QStringLiteral("Wrong message type. Http Server only accepts 'Request' type."));
      qDebug() << QStringLiteral("Wrong message type. Http Server only accepts 'Request' type.");
      emit sHttpResponseProcessed(cHttpError);
    }
  }

  QVariantMap NConnectionController::fApiRoles() const {
    QVariantMap lApiRolesMap;
    QStringList lApiRoles(QString(APP_ROLES).split(":"));
    for(QString& lApiRole : lApiRoles) {
      lApiRole.remove(' ');
      QString lRole = lApiRole.split("[").at(0);
      QStringList lSupportedVersions = lApiRole.split("[").at(1).split("]").at(0).split(",");
      lApiRolesMap[lRole] = lSupportedVersions;
    }
    return lApiRolesMap;
  }

  void NConnectionController::fFillMethodDescriptions() {
    fAddMethodFunctionDescription("setmaxconnections", tr("Sets the maximum number of client connections that should be accepted.\nParameters:\n  maxconnections [0- ]: Maximum connections allowed. 0 means no limit."));
    fAddMethodFunctionDescription("getmaxconnections", tr("Gets the maximum number of client connections currently accepted.\nReturn: [0- ] - 0 means unlimited connections."));
    fAddMethodFunctionDescription("gettotalconnections", tr("Gets the total connection count currently established.\nReturn: [0- ]"));
    fAddMethodFunctionDescription("getcompressionlevel", tr("Gets the current compression level for client connections.\nReturn: [0-9] - 0 is no compression, while 9 means maximum compression."));
    fAddMethodFunctionDescription("setcompressionlevel", tr("Sets the compression level that packets will have with client communication channels.\nParameters:\n  compressionlevel [0-9] - 0 means no compression while 9 is maximum compression."));
  }

  void NConnectionController::fFillMethodMinEventAndMinPeriod() {
    fAddMethodMinEventAndMinPeriod("setmaxconnections", QString("0,0"));
    fAddMethodMinEventAndMinPeriod("getmaxconnections", QString("1,0"));
    fAddMethodMinEventAndMinPeriod("gettotalconnections", QString("1,0"));
    fAddMethodMinEventAndMinPeriod("getcompressionlevel", QString("1,0"));
    fAddMethodMinEventAndMinPeriod("setcompressionlevel", QString("0,0"));
  }

  void NConnectionController::fProcessResponse(const QVariantMap& lMessageResponse) {
    bool lResponseSuccessfull = (NMessageResponse::EResponseStatus(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseStatusFieldName).toInt()) == NMessageResponse::EResponseStatus::eResponseSuccessful);
    QString lRequestID(lMessageResponse.value(cFieldName_MessageData).toMap().value(cRequestIDFieldName).toString());

    QVariantMap lResponseData(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseDataFieldName).toMap());
    if(mForwardedMessages.contains(lRequestID)) {
      TMessageRequestToProcess lMessageResponseStructure = mForwardedMessages.value(lRequestID);
      qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageResponseStructure.mMSecsSinceEpoch);
      NMessageResponse* rRequestResponse = new NMessageResponse(lMessageResponseStructure.mWebSocketID, QString(), lMessageResponseStructure.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseSuccessful,
                                           tr(""), 0, QVariantMap({{lMessageResponseStructure.mOriginalMethodName, lResponseData.value(lMessageResponseStructure.mOriginalMethodName).toMap() }} ), QString());
      fSendMessage(lMessageResponseStructure.mWebSocketsServerName, rRequestResponse);

      if(!lMessageResponseStructure.mSubscriptionPeriod && !lMessageResponseStructure.mSubscriptionEventCounter)
         mForwardedMessages.remove(lRequestID);
      return;
    }
    if(lResponseData.contains(cFieldName_RegisterAPI) && lResponseSuccessfull) {
      QVariantMap lGetConsolidatedAPI;
      lGetConsolidatedAPI[cCommand_GetConsolidatedAPI] = QVariant();
      fSendMessage(cServiceManagerName, new NMessageRequest(cServiceManagerName, QString(), false, 1, 0, QString(), 0, lGetConsolidatedAPI, this), NWebSocket::EConnectionState::eConnectionActive);
      return;
    }
    if(lResponseData.contains(cCommand_GetConsolidatedAPI)) {
      lResponseSuccessfull |= (NMessageResponse::EResponseStatus(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseStatusFieldName).toInt()) == NMessageResponse::EResponseStatus::eResponseMethodExistedError);
      if(lResponseSuccessfull) {
        mPrivateMethods = lResponseData.value(cCommand_GetConsolidatedAPI).toMap().value(cFieldName_Private).toMap();
        mPublicMethods = lResponseData.value(cCommand_GetConsolidatedAPI).toMap().value(cFieldName_Public).toMap();
        mAdminMethods = lResponseData.value(cCommand_GetConsolidatedAPI).toMap().value(cFieldName_Admin).toMap();
        return;
      }
    }

  }

  void NConnectionController::fSendMessage(const QString& lWebSocketsID, NMessage* rMessage, NWebSocket::EConnectionState lMinStateRequired) {
    if(lWebSocketsID == cHttpServerName) {
      emit sHttpResponseProcessed(rMessage->fToJsonString());
      rMessage->deleteLater();
    } else {
      NCoreService::fSendMessage(lWebSocketsID, rMessage, lMinStateRequired);
    }
  }

  void NConnectionController::ListAPI(const TMessageRequestToProcess& lMessageRequest) {
      QVariantMap lListAPIResponse { {"ListAPI", QVariantMap() } };
      if((lMessageRequest.mWebSocketsServerName == cClientServerName) || lMessageRequest.mWebSocketsServerName == cHttpServerName) {
        lListAPIResponse["ListAPI"] = mPublicMethods;
      }
      if(lMessageRequest.mWebSocketsServerName == cAdminServerName) {
        QVariantMap lCurrentMap(mPublicMethods);
        lCurrentMap = lCurrentMap.unite(mAdminMethods);
        lListAPIResponse["ListAPI"] = lCurrentMap;
      }
      if(lMessageRequest.mWebSocketsServerName == cCommServerName) {
        QVariantMap lCurrentMap(mPublicMethods);
        lCurrentMap = lCurrentMap.unite(mAdminMethods);
        lCurrentMap = lCurrentMap.unite(mPrivateMethods);
        lListAPIResponse["ListAPI"] = lCurrentMap;
      }

    qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
    NMessageResponse* lResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseSuccessful, QString(), 0, lListAPIResponse, QString());
    fSendMessage(lMessageRequest.mWebSocketsServerName, lResponse);
  }

  void NConnectionController::fInvokeMethod(TMessageRequestToProcess& lMessageRequestToProcess) {
    if(lMessageRequestToProcess.mEffectiveMethodName == QString("ListAPI"))
      NCoreService::fInvokeMethod(lMessageRequestToProcess);
    else {
      if(lMessageRequestToProcess.mWebSocketsServerName == cAdminServerName) {
        if(fMethodValid(lMessageRequestToProcess.mEffectiveMethodName, mAdminMethods) || fMethodValid(lMessageRequestToProcess.mEffectiveMethodName, mPublicMethods))
          fForwardMessage(lMessageRequestToProcess);  
        else 
          fSendBadMethodResponse(lMessageRequestToProcess);  
      }
      if(lMessageRequestToProcess.mWebSocketsServerName == cCommServerName) {
        if(fMethodValid(lMessageRequestToProcess.mEffectiveMethodName, mAdminMethods) || fMethodValid(lMessageRequestToProcess.mEffectiveMethodName, mPublicMethods) || fMethodValid(lMessageRequestToProcess.mEffectiveMethodName, mPrivateMethods))
          fForwardMessage(lMessageRequestToProcess);  
        else 
          fSendBadMethodResponse(lMessageRequestToProcess);
      }
      if((lMessageRequestToProcess.mWebSocketsServerName == cClientServerName) || lMessageRequestToProcess.mWebSocketsServerName == cHttpServerName) {
        if(fMethodValid(lMessageRequestToProcess.mEffectiveMethodName, mPublicMethods))
          fForwardMessage(lMessageRequestToProcess);  
        else 
          fSendBadMethodResponse(lMessageRequestToProcess);
      }
    }
  }

  bool NConnectionController::fMethodValid(const QString& lMethodName, const QVariantMap& lMethodList) {
    QMapIterator<QString, QVariant> i1(lMethodList);
    while(i1.hasNext()) {
      i1.next();
      QStringList lMethod(i1.key().split("."));
      if(lMethod.size() != 2) {
        qDebug("%s", qUtf8Printable(QString("API method does not contain domain!")));
        continue;
      }
      if(lMethodName == lMethod.at(1))
        return true;
    }
    return false;
  }

  void NConnectionController::fForwardMessage(const TMessageRequestToProcess& lMessageRequestToProcess) {
    QVariantMap lParameters({{QStringLiteral("ForwardMessage"), QVariantMap( {{lMessageRequestToProcess.mEffectiveMethodName, lMessageRequestToProcess.mParameters }}) }} );
    NMessageRequest* rForwardedRequest =   new  NMessageRequest(cServiceManagerName, QString(), false, lMessageRequestToProcess.mSubscriptionEventCounter, lMessageRequestToProcess.mSubscriptionPeriod, QString(), 0, lParameters, this);
    mForwardedMessages.insert(rForwardedRequest->fMessageID(), lMessageRequestToProcess);
    fSendMessage(cServiceManagerName, rForwardedRequest);
  }
  
  void NConnectionController::fSendBadMethodResponse(const TMessageRequestToProcess& lMessageRequestToProcess) {
    qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequestToProcess.mMSecsSinceEpoch);
    NMessageResponse* rRequestResponse = new NMessageResponse(lMessageRequestToProcess.mWebSocketID, QString(), lMessageRequestToProcess.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseNoMethodError,
                                         tr("Method '%1' not found in current channel!").arg(lMessageRequestToProcess.mEffectiveMethodName), 0, QVariantMap({{lMessageRequestToProcess.mOriginalMethodName, QVariantMap()}} ),
                                         QString("%1-%2").arg(fAbbreviation()).arg(int(NMessageResponse::EResponseStatus::eResponseNoMethodError)));
    fSendMessage(lMessageRequestToProcess.mWebSocketsServerName, rRequestResponse);
  }
}

