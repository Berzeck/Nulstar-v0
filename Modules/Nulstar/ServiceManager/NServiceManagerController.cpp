#include <QHostAddress>
#include <QSettings>
#include <QWebSocket>
#include "NServiceManagerController.h"

namespace NulstarNS {
  const int cTimeSeconds_DependencesSearchRetryPeriod = 1;
  const int cTotal_DependencesSearchRetryTimes = 9;

  NServiceManagerController::NServiceManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress &lIP, const QUrl &lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                           : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);
    connect(&mFindDependenciesRetryTimer, &QTimer::timeout, this, &NServiceManagerController::fFindDependencies);
    mFindDependenciesRetryTimer.start(1000 * cTimeSeconds_DependencesSearchRetryPeriod);
  }

  QVariantMap NServiceManagerController::fApiRoles() const {
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

  void NServiceManagerController::fOnWebSocketDisconnected(const QString& lWebSocketID) {
    QMapIterator<QString, NModuleAPI> i1(mModuleAPIActive);
    while(i1.hasNext()) {
      i1.next();
      if(lWebSocketID == i1.value().fWebSocketID()) {
        QMapIterator<QString, NModuleAPI> i2(mModuleAPIActive);
        while(i2.hasNext()) {
          i2.next();
          if((lWebSocketID != i2.value().fWebSocketID()) && i2.value().fAreRolesNeeded(i1.value().fModuleRoles())) {
            fCloseConnection(i2.value().fWebSocketServerName(), i2.value().fWebSocketID());
          }
        }

        qDebug("%s", qUtf8Printable(QString("API from module '%1' using WebSocket connection '%2' has been removed!").arg(i1.value().fModuleName()).arg(i1.value().fWebSocketID())));
        mModuleAPIActive.remove(i1.key());
        emit sEventTriggered("getconsolidatedapi");
        break;
      }
    }
    for(const NModuleAPI& lModuleAPI : mModuleAPIPendingDependencies) {
      if(lModuleAPI.fWebSocketID() == lWebSocketID) {
        QMapIterator<QString, NModuleAPI> i2(mModuleAPIActive);
        while(i2.hasNext()) {
          i2.next();
          if((lWebSocketID != i2.value().fWebSocketID()) && i2.value().fAreRolesNeeded(lModuleAPI.fModuleRoles())) {
            fCloseConnection(i2.value().fWebSocketServerName(), i2.value().fWebSocketID());
          }
        }

        qDebug("%s", qUtf8Printable(QString("Pending API from module '%1' using WebSocket connection '%2' has been removed!").arg(lModuleAPI.fModuleName()).arg(lModuleAPI.fWebSocketID())));
        mModuleAPIPendingDependencies.removeOne(lModuleAPI);
        emit sEventTriggered("getconsolidatedapi");
        break;
      }
    }
  }

  void NServiceManagerController::fFindDependencies() {
    for(NModuleAPI& lModuleAPIPending : mModuleAPIActive) {
      QSettings lTempLog("ActiveModules.log", QSettings::IniFormat);
      lTempLog.setValue(lModuleAPIPending.fModuleName(), QTime::currentTime().toString("hh:mm:ss"));
    }
    for(NModuleAPI& lModuleAPIPending : mModuleAPIPendingDependencies) {
      if(lModuleAPIPending.fFindDependenciesRetryCounter() >= cTotal_DependencesSearchRetryTimes) {
         QVariantMap lDependencies;
         QMapIterator<QString, QVariant> i1(lModuleAPIPending.fDependencies());
         while(i1.hasNext()) {
           i1.next();
           QVariantMap lParams;
           lParams[cFieldName_ModuleRoleVersion] = i1.value();
           lParams[cFieldName_IP] = QString();
           lParams[cFieldName_Port] = QString();
           lDependencies[i1.key()] = lParams;
         }
         QVariantMap lResponseMap({ {cFieldName_RegisterAPI, { QVariantMap( {{ cFieldName_Dependencies, lDependencies }} ) }}} );
         qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lModuleAPIPending.fMSecsSinceEpoch());

         NMessageResponse* lRegisterAPIResponse = new NMessageResponse(lModuleAPIPending.fWebSocketID(), QString(), lModuleAPIPending.fMessageID(), lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseDependenciesNoFoundError,
                                               tr("Dependencies of module '%1' not found!").arg(lModuleAPIPending.fModuleName()), 0,lDependencies);

         fSendMessage(lModuleAPIPending.fWebSocketServerName(), lRegisterAPIResponse);
         fCloseConnection(lModuleAPIPending.fWebSocketServerName(), lModuleAPIPending.fWebSocketID());
         mModuleAPIPendingDependencies.removeOne(lModuleAPIPending);
      }
      else {
        QVariantMap lDependencies;
        QMapIterator<QString, QVariant> i1(lModuleAPIPending.fDependencies());

        bool lAllDependenciesSatisfied = true;
        while(i1.hasNext() && lAllDependenciesSatisfied) {
          i1.next();
          QString lRoleName = i1.key();
          QStringList lVersionsSupported(i1.value().toString());
          NModuleAPIRole lDependency(lRoleName, lVersionsSupported);
          bool lDependencySatisfied = false;
          QMapIterator<QString, NModuleAPI> i2(mModuleAPIActive);
          while(i2.hasNext()) {
            i2.next();
            if(i2.value().fIsRoleSupported(lDependency)) {
              lDependencySatisfied = true;
              QVariantMap lConnectionData( {{i1.key(), QVariantMap( {{cFieldName_IP, i2.value().fIP().toString() }, {cFieldName_Port, QString::number(i2.value().fPort()) },
                                                                     {cFieldName_ModuleRoleVersion, i1.value().toString() } } ) }});
              if(lDependencies.size())
                lDependencies[cFieldName_RegisterAPI] = QVariantMap( {{cFieldName_Dependencies, lDependencies[cFieldName_RegisterAPI].toMap()[cFieldName_Dependencies].toMap().unite(lConnectionData) }} );
              else
                lDependencies[cFieldName_RegisterAPI] = QVariantMap( {{cFieldName_Dependencies, lConnectionData }} );
              break;
            }
          }
          for(NModuleAPI& lModuleAPIPendingDependency : mModuleAPIPendingDependencies) {
            if(lModuleAPIPendingDependency.fIsRoleSupported(lDependency)) {
              lDependencySatisfied = true;
              QVariantMap lConnectionData( {{i1.key(), QVariantMap( {{cFieldName_IP, lModuleAPIPendingDependency.fIP().toString() }, {cFieldName_Port, QString::number(lModuleAPIPendingDependency.fPort()) },
                                                                   {cFieldName_ModuleRoleVersion, i1.value().toString() } } ) }});
              if(lDependencies.size())
                lDependencies[cFieldName_RegisterAPI] = QVariantMap( {{cFieldName_Dependencies, lDependencies[cFieldName_RegisterAPI].toMap()[cFieldName_Dependencies].toMap().unite(lConnectionData) }} );
              else
                lDependencies[cFieldName_RegisterAPI] = QVariantMap( {{cFieldName_Dependencies, lConnectionData }} );
              break;
            }
          }

          if(!lDependencySatisfied) {
            lAllDependenciesSatisfied = false;
            lModuleAPIPending.fSetFindDependenciesRetryCounter(lModuleAPIPending.fFindDependenciesRetryCounter() + 1);
            break;
          }
        }
        if(lAllDependenciesSatisfied) {
          QString lModuleAPIIndexName(QString("%1|%2").arg(lModuleAPIPending.fModuleName()).arg(lModuleAPIPending.fWebSocketID()));
          qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lModuleAPIPending.fMSecsSinceEpoch());
          NMessageResponse* lRegisterAPIResponse = new NMessageResponse(lModuleAPIPending.fWebSocketID(), QString(), lModuleAPIPending.fMessageID(), lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseSuccessful,
                                                  tr("Module '%1' is active!").arg(lModuleAPIPending.fModuleName()), 0,lDependencies);
          fSendMessage(lModuleAPIPending.fWebSocketServerName(), lRegisterAPIResponse);
          mModuleAPIActive[lModuleAPIIndexName] = lModuleAPIPending;
          mModuleAPIPendingDependencies.removeOne(lModuleAPIPending);
          emit sEventTriggered("getconsolidatedapi");
        }
      }
    }
  }

  void NServiceManagerController::getconsolidatedapi(const TMessageRequestToProcess& lMessageRequest) {
    QVariantMap lMethods;
    lMethods[cFieldName_Admin] = QVariantMap();
    lMethods[cFieldName_Private] = QVariantMap();
    lMethods[cFieldName_Public] = QVariantMap();

    NMessageResponse::EResponseStatus lConsolidationSuccess = NMessageResponse::EResponseStatus::eResponseSuccessful;
    QString lLastErrorMessage;
    QMapIterator<QString, NModuleAPI> i1(mModuleAPIActive);
    while(i1.hasNext()) {
      i1.next();
      QString lModuleDomain(i1.value().fModuleDomain());
      QString lModuleName(i1.value().fModuleName());
//      QVersionNumber lModuleVersion(QVersionNumber::fromString(i1.value().fModuleVersion()));

      for(const QVariant& lApiMethod : i1.value().fMethods()) {
        QVariantMap lMethod(lApiMethod.toMap());
        QString lMethodName(lMethod.value(cFieldName_MethodName).toString());
        QString lMethodScope(lMethod.value(cFieldName_MethodScope).toString());
        QString lMethodEffectiveName(QString("%1.%2").arg(lModuleDomain).arg(lMethodName));
        QString lMethodEffectiveScope;
        if(lMethodScope == cFieldName_Admin.toLower())
          lMethodEffectiveScope = cFieldName_Admin;
        if(lMethodScope == cFieldName_Public.toLower())
          lMethodEffectiveScope = cFieldName_Public;
        if(lMethodScope == cFieldName_Private.toLower())
          lMethodEffectiveScope = cFieldName_Private;

        if(lMethods.value(lMethodEffectiveScope).toMap().contains(lMethodEffectiveName)) {
          lConsolidationSuccess = NMessageResponse::EResponseStatus::eResponseMethodExistedError;
          lLastErrorMessage = tr("Method called '%1' from module '%2' already exists!!").arg(lMethodEffectiveName).arg(lModuleName);
          qDebug("%s", qUtf8Printable(lLastErrorMessage));
        }
        else {
          QVariantMap lCurrentMethods = lMethods.value(lMethodEffectiveScope).toMap();
          lCurrentMethods.insert(lMethodEffectiveName, lMethod);
          lMethods[lMethodEffectiveScope] = lCurrentMethods;
        }
      }
    }
    QVariantMap lGetConsolidatedAPIResponse { {cCommand_GetConsolidatedAPI, lMethods } };
    qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
    NMessageResponse* lConsolidateAPIResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, lConsolidationSuccess, lLastErrorMessage, 0, lGetConsolidatedAPIResponse);
    fSendMessage(lMessageRequest.mWebSocketsServerName, lConsolidateAPIResponse);
  }

  void NServiceManagerController::registerapi(const TMessageRequestToProcess& lMessageRequest) {
     NModuleAPI lModuleAPI(lMessageRequest.mParameters);
     lModuleAPI.fSetMessageID(lMessageRequest.mMessageID);
     lModuleAPI.fSetMSecsSinceEpoch(lMessageRequest.mMSecsSinceEpoch);
     lModuleAPI.fSetWebSocketID(lMessageRequest.mWebSocketID);
     lModuleAPI.fSetWebSocketServerName(lMessageRequest.mWebSocketsServerName);
     if(lModuleAPI.fIsValid())
       mModuleAPIPendingDependencies << lModuleAPI;
  }

  void NServiceManagerController::forwardmessage(const TMessageRequestToProcess& lMessageRequest) {
    //QVariantMap lParameters({{QStringLiteral("forwardmessage"), lMessageRequestToProcess.mParameters}} );
    //TMessageRequestToProcess lForwardedMessage({ lMessageRequest.mWebSocketsServerName, lMessageRequest.mWebSocketID, lMessageRequest.mMessageID,   });
    QStringList lMethods = lMessageRequest.mParameters.keys();
    if(lMethods.size()) {
      QMapIterator<QString, NModuleAPI> i1(mModuleAPIActive);
      while(i1.hasNext()) {
        i1.next();
        if(i1.value().fIsMethodSupported(lMethods.at(0))) {
          QVariantMap lParameters( {{lMethods.at(0), lMessageRequest.mParameters.value(lMethods.at(0)).toMap() }} );
          NMessageRequest* rForwardedRequest = new NMessageRequest(i1.value().fWebSocketID(), QString(), false, lMessageRequest.mSubscriptionEventCounter, lMessageRequest.mSubscriptionPeriod, QString(), 0, lParameters, this);
          TMessageRequestToProcess lMessageRequestOriginal(lMessageRequest);
          lMessageRequestOriginal.mOriginalMethodName = lMethods.at(0);
          lMessageRequestOriginal.mEffectiveMethodName = lMethods.at(0);
          mForwardedMessages.insert(rForwardedRequest->fMessageID(), lMessageRequestOriginal);
          fSendMessage(i1.value().fWebSocketServerName(), rForwardedRequest);
          return;
        }
      }
      qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
      NMessageResponse* rRequestResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseMethodUnavailableError,
                                           tr("Method '%1' not available anymore!").arg(lMethods.at(0)), 0, QVariantMap({{lMessageRequest.mOriginalMethodName, QVariantMap()}} ));
      fSendMessage(lMessageRequest.mWebSocketsServerName, rRequestResponse);
      return;
    }
    qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
    NMessageResponse* rRequestResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseMethodListEmptyError,
                                         tr("Method list is empty!"), 0, QVariantMap({{lMessageRequest.mOriginalMethodName, QVariantMap()}} ));
    fSendMessage(lMessageRequest.mWebSocketsServerName, rRequestResponse);
  }

  void NServiceManagerController::fProcessResponse(const QVariantMap& lMessageResponse) {
  //  bool lResponseSuccessfull(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseStatusFieldName).toBool());
    QString lRequestID(lMessageResponse.value(cFieldName_MessageData).toMap().value(cRequestIDFieldName).toString());
    QVariantMap lResponseData(lMessageResponse.value(cFieldName_MessageData).toMap().value(cResponseDataFieldName).toMap());
    if(mForwardedMessages.contains(lRequestID)) {
      TMessageRequestToProcess lMessageResponseStructure = mForwardedMessages.value(lRequestID);
      QVariantMap lParams(lResponseData.value(lMessageResponseStructure.mOriginalMethodName).toMap());
      qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageResponseStructure.mMSecsSinceEpoch);
      NMessageResponse* rRequestResponse = new NMessageResponse(lMessageResponseStructure.mWebSocketID, QString(), lMessageResponseStructure.mMessageID, lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseSuccessful,
                                           tr(""), 0, QVariantMap({{lMessageResponseStructure.mOriginalMethodName, lParams }} ));
      fSendMessage(lMessageResponseStructure.mWebSocketsServerName, rRequestResponse);

      if(!lMessageResponseStructure.mSubscriptionPeriod && !lMessageResponseStructure.mSubscriptionEventCounter)
        mForwardedMessages.remove(lRequestID);
      return;
    }
  }
}
