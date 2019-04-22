#include <QHostAddress>
#include <QSettings>
#include <QStringList>
#include <QWebSocket>
#include <NMessage.h>
#include <NWebSocketServer.h>
#include <NApiBuilder.h>

#include "NConnectionController.h"

namespace NulstarNS {
  NConnectionController::NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl &lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, quint16 lAdminPort, quint16 lClientPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                       : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent), mRequestID(0), mCompressionLevel(0) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);
    fAddWebSocketServer(lAdminPort, QHostAddress::AnyIPv4, cAdminServerName, cAdminServerLabel, false);
    fAddWebSocketServer(lClientPort, QHostAddress::AnyIPv4, cClientServerName, cClientServerLabel, false);
    fFillMethodDescriptions();
    fFillMethodMinEventAndMinPeriod();
  }
/***
  NResponse NConnectionController::setcompressionlevel(quint8 lCompressionLevel) {
    if(lCompressionLevel > 9) {
      NResponse lResponse(false, false, tr("Compression level should be in the range [0-9]"));
      return lResponse;
    }
    mCompressionLevel = lCompressionLevel;
    NResponse lResponse(true, true, tr("Compression level is set to '%1'").arg(mCompressionLevel));
    return lResponse;
  }

  NResponse NConnectionController::getcompressionlevel() {
    NResponse lResponse(true, mCompressionLevel, tr("Compression level is '%1'").arg(mCompressionLevel));
    return lResponse;
  } ***/

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
    if(lResponseData.contains(cCommand_GetConsolidatedAPI) && lResponseSuccessfull) {
      mPrivateMethods = lResponseData.value(cCommand_GetConsolidatedAPI).toMap().value(cFieldName_Private).toMap();
      mPublicMethods = lResponseData.value(cCommand_GetConsolidatedAPI).toMap().value(cFieldName_Public).toMap();
      mAdminMethods = lResponseData.value(cCommand_GetConsolidatedAPI).toMap().value(cFieldName_Admin).toMap();
      return;
    }

  }

  void NConnectionController::listapi(const TMessageRequestToProcess& lMessageRequest) {
      QVariantMap lListAPIResponse { {"listapi", QVariantMap() } };
      if(lMessageRequest.mWebSocketsServerName == cClientServerName) {
        lListAPIResponse["listapi"] = mPublicMethods;
      }
      if(lMessageRequest.mWebSocketsServerName == cAdminServerName) {
        QVariantMap lCurrentMap(mPublicMethods);
        lCurrentMap = lCurrentMap.unite(mAdminMethods);
        lListAPIResponse["listapi"] = lCurrentMap;
      }
      if(lMessageRequest.mWebSocketsServerName == cAdminServerName) {
        QVariantMap lCurrentMap(mPublicMethods);
        lCurrentMap = lCurrentMap.unite(mAdminMethods);
        lCurrentMap = lCurrentMap.unite(mPrivateMethods);
        lListAPIResponse["listapi"] = lCurrentMap;
      }

    qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
    NMessageResponse* lResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseSuccessful, QString(), 0, lListAPIResponse, QString());
    fSendMessage(lMessageRequest.mWebSocketsServerName, lResponse);
  }

  void NConnectionController::fInvokeMethod(TMessageRequestToProcess& lMessageRequestToProcess) {
    if(lMessageRequestToProcess.mEffectiveMethodName == QString("listapi"))
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
      if(lMessageRequestToProcess.mWebSocketsServerName == cClientServerName) {
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
    QVariantMap lParameters({{QStringLiteral("forwardmessage"), QVariantMap( {{lMessageRequestToProcess.mEffectiveMethodName, lMessageRequestToProcess.mParameters }}) }} );
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

