#include <QHostAddress>
#include <QWebSocket>
#include "NServiceManagerController.h"

namespace NulstarNS {
  NServiceManagerController::NServiceManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks, QObject* rParent)
                           : NCoreService(lLogLevel, lAllowedNetworks, rParent) {
    NCoreService::mLogLevel = lLogLevel;
    pWebCommServer = new NWebSocketServer(QStringLiteral("Nulstar Internal Communication"), lSslMode, this);
  }

  NServiceManagerController::~NServiceManagerController() {
    pWebCommServer->close();
  }

  void NServiceManagerController::fControlCommServer(EServiceAction lAction, quint16 lCommPort, QHostAddress::SpecialAddress lAddress ) {
    if(lAction == EServiceAction::eStartService) {
      pWebCommServer->fListen(lAddress, lCommPort);
    }
    if(lAction == EServiceAction::eStopService) {
      pWebCommServer->close();
    }
    if(lAction == EServiceAction::eChangePort) {
      pWebCommServer->fSetPort(lCommPort);
    }
  }
}
