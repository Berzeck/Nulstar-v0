#include <QHostAddress>
#include <QWebSocket>
#include "NServiceManagerController.h"

namespace NulstarNS {
  NServiceManagerController::NServiceManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress &lIP, const QUrl &lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                           : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent) {

     if(lCommPort)
       fAddWebSocketServer(lCommPort, lBindAddress);
  }

  QVariantMap NServiceManagerController::fApiRoles() const {
    QVariantMap lApiRolesMap;
    QStringList lApiRoles(QString(APP_ROLES).split(":"));
    for(QString& lApiRole : lApiRoles) {
      lApiRole.remove(' ');
      QString lRole = lApiRole.split("[").at(0);
      QStringList lSupportedVersions = lApiRole.split("[").at(1).split("]").at(0).split(",");
      lApiRolesMap[lApiRole] = lSupportedVersions;
    }
    return lApiRolesMap;
  }

  void NServiceManagerController::fOnWebSocketDisconnected(const QString& lWebSocketID) {
    QMapIterator<QString, NModuleAPI> i1(mModuleAPISet);
    while(i1.hasNext()) {
      i1.next();
      if(lWebSocketID == i1.value().fWebSocketID()) {
        mModuleAPISet.remove(i1.key());
        qDebug("%s", qUtf8Printable(QString("API from module '%1' usng WebSocket connection '%2' has been removed!").arg(i1.value().fModuleName()).arg(i1.value().fWebSocketID())));
      }
    }
  }

  void NServiceManagerController::registerapi(const QString& lWebSocketsServerName, const QString& lWebSocketID, const QString& lMessageID, const QVariantMap& lParameters) {
     NModuleAPI lModuleAPI(lParameters);
     lModuleAPI.fSetMessageID(lMessageID);
     lModuleAPI.fSetWebSocketID(lWebSocketID);
     lModuleAPI.fSetWebSocketServerName(lWebSocketsServerName);
     mModuleAPISet[lModuleAPI.fModuleName()] = lModuleAPI;
  }
}
