#include <QHostAddress>
#include <QWebSocket>
#include "NConnectionController.h"


namespace NulstarNS {
  NConnectionController::NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks, QObject* rParent)
                       : NCoreService(lLogLevel, lAllowedNetworks, rParent) {
    NCoreService::mLogLevel = lLogLevel;
    pWebAdminServer = new NWebSocketServer(QStringLiteral("Nulstar Admin"), lSslMode, this);
    pWebClientServer = new NWebSocketServer(QStringLiteral("Nulstar Client"), lSslMode, this);
    pWebCommServer = new NWebSocketServer(QStringLiteral("Nulstar Internal Communication"), lSslMode, this);
  }

  NConnectionController::~NConnectionController() {
    pWebAdminServer->close();
    pWebClientServer->close();    
    pWebCommServer->close();
  }

  void NConnectionController::fControlAdminServer(EServiceAction lAction, quint16 lAdminPort, QHostAddress::SpecialAddress lAddress ) {
    if(lAction == EServiceAction::eStartService) {
      pWebAdminServer->fListen(lAddress, lAdminPort);
    }
    if(lAction == EServiceAction::eStopService) {
      pWebAdminServer->close();
    }
    if(lAction == EServiceAction::eChangePort) {
      pWebAdminServer->fSetPort(lAdminPort);
    }
  }

  void NConnectionController::fControlClientServer(EServiceAction lAction, quint16 lClientPort, QHostAddress::SpecialAddress lAddress ) {
    if(lAction == EServiceAction::eStartService) {
      pWebClientServer->fListen(lAddress, lClientPort);
    }
    if(lAction == EServiceAction::eStopService) {
      pWebClientServer->close();
    }
    if(lAction == EServiceAction::eChangePort) {
      pWebClientServer->fSetPort(lClientPort);
    }
  }

  void NConnectionController::fControlCommServer(EServiceAction lAction, quint16 lCommPort, QHostAddress::SpecialAddress lAddress ) {
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

