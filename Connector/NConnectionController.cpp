#include <QHostAddress>
#include <QSettings>
#include <QWebSocket>
#include <NWebSocketServer.h>
#include <NApiBuilder.h>
#include "NConnectionController.h"

namespace NulstarNS {
  NConnectionController::NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lServiceManagerIP, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, quint16 lAdminPort, quint16 lClientPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                       : NCoreService(lSslMode, lLogLevel, lServiceManagerIP, lAllowedNetworks, lCommPort, lBindAddress, rParent), mRequestID(0), mCompressionLevel(0) {

    fAddWebSocketServer(lAdminServerName, lAdminServerLabel, lAdminPort, QHostAddress::Any, false);
    fAddWebSocketServer(lClientServerName, lClientServerLabel, lClientPort, QHostAddress::Any, false);

    NResponse fMaxConnections(quint64 lID, QString lExternalID, const QString &lName);
    NResponse fTotalConnections(quint64 lID, QString lExternalID, const QString &lName);
    NResponse fSetMaxConnections(quint64 lID, QString lExternalID, const QString& lName, int lMaxconnections);
  }

  NResponse NConnectionController::setcompressionlevel(quint64 lID, QString lExternalID, quint8 lCompressionLevel) {
    if(lCompressionLevel > 9) {
      NResponse lResponse(lID, lExternalID, false, false, tr("Compression level should be in the range [0-9]"));
      return lResponse;
    }
    mCompressionLevel = lCompressionLevel;
    NResponse lResponse(lID, lExternalID, true, true, tr("Compression level is set to '%1'").arg(mCompressionLevel));
    return lResponse;
  }

  NResponse NConnectionController::getcompressionlevel(quint64 lID, QString lExternalID) {
    NResponse lResponse(lID, lExternalID, true, mCompressionLevel, tr("Compression level is '%1'").arg(mCompressionLevel));
    return lResponse;
  }

  void NConnectionController::fFillMethodDescriptions() {
    fAddMethodFunctionDescription("setmaxconnections", tr("Sets the maximum number of client connections that should be accepted.\nParameters:\n  maxconnections [0- ]: Maximum connections allowed. 0 means no limit."));
    fAddMethodFunctionDescription("getmaxconnections", tr("Gets the maximum number of client connections currently accepted.\nReturn: [0- ] - 0 means unlimited connections."));
    fAddMethodFunctionDescription("gettotalconnections", tr("Gets the total connection count currently established.\nReturn: [0- ]"));
    fAddMethodFunctionDescription("getcompressionlevel", tr("Gets the current compression level for client connections.\nReturn: [0-9] - 0 is no compression, while 9 means maximum compression."));
    fAddMethodFunctionDescription("setcompressionlevel", tr("Sets the compression level that packets will have with client communication channels.\nParameters:\n  compressionlevel [0-9] - 0 means no compression while 9 is maximum compression."));
  }
}

