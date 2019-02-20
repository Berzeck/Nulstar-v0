#include <QHostAddress>
#include <QWebSocket>
#include "NServiceManagerController.h"

namespace NulstarNS {
  const int cTimeSeconds_DependencesSearchRetryPeriod = 1;
  const int cTotal_DependencesSearchRetryTimes = 4;

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
        qDebug("%s", qUtf8Printable(QString("API from module '%1' using WebSocket connection '%2' has been removed!").arg(i1.value().fModuleName()).arg(i1.value().fWebSocketID())));
        mModuleAPIActive.remove(i1.key());
      }
    }
    for(const NModuleAPI& lModuleAPI : mModuleAPIPendingDependencies) {
      if(lModuleAPI.fWebSocketID() == lWebSocketID) {
        mModuleAPIPendingDependencies.removeOne(lModuleAPI);
      }
    }
  }

  void NServiceManagerController::fFindDependencies() {
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
         QVariantMap lReponseMap({ {cFieldName_RegisterAPI, { QVariantMap( {{ cFieldName_Dependencies, lDependencies }} ) }}} );
         qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lModuleAPIPending.fMSecsSinceEpoch());
         NMessageResponse* lRegisterAPIResponse = new NMessageResponse(lModuleAPIPending.fWebSocketID(), QString(), lModuleAPIPending.fMessageID(), lResponseProcessingTime, NMessageResponse::EResponseStatus::eResponseError,
                                               tr("Dependencies of module '%1' not found!").arg(lModuleAPIPending.fModuleName()), 0,lDependencies);
         fSendMessage(lModuleAPIPending.fWebSocketServerName(), lRegisterAPIResponse);
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
        }
      }
    }
  }

  void NServiceManagerController::registerapi(const QString& lWebSocketsServerName, const QString& lWebSocketID, const QString& lMessageID, const QVariantMap& lParameters, qint64 lMSecsSinceEpoch) {
     NModuleAPI lModuleAPI(lParameters);
     lModuleAPI.fSetMessageID(lMessageID);
     lModuleAPI.fSetMSecsSinceEpoch(lMSecsSinceEpoch);
     lModuleAPI.fSetWebSocketID(lWebSocketID);
     lModuleAPI.fSetWebSocketServerName(lWebSocketsServerName);
     if(lModuleAPI.fIsValid())
       mModuleAPIPendingDependencies << lModuleAPI;
  }
}
