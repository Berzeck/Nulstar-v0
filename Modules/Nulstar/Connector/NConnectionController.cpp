#include <QHostAddress>
#include <QSettings>
#include <QStringList>
#include <QWebSocket>
#include <NWebSocketServer.h>
#include <NApiBuilder.h>
#include "NConnectionController.h"

namespace NulstarNS {
  NConnectionController::NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lIP, const QUrl &lServiceManagerUrl, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, quint16 lAdminPort, quint16 lClientPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                       : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lAllowedNetworks, rParent), mRequestID(0), mCompressionLevel(0) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);
    fAddWebSocketServer(lAdminPort, QHostAddress::Any, lAdminServerName, lAdminServerLabel, false);
    fAddWebSocketServer(lClientPort, QHostAddress::Any, lClientServerName, lClientServerLabel, false);
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
}

