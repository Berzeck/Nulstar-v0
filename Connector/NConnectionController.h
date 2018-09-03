#ifndef NCONNECTIONCONTROLLER_H
#define NCONNECTIONCONTROLLER_H

#include <QHostAddress>
#include <QObject>
#include <NCoreService.h>
#include <NWebSocketServer.h>

namespace NulstarNS {
  class NConnectionController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), QObject* rParent = nullptr);
      ~NConnectionController();

    private:      
      NWebSocketServer* pWebAdminServer;
      NWebSocketServer* pWebClientServer;
      NWebSocketServer* pWebCommServer;

    public Q_SLOTS:
      void fControlAdminServer(EServiceAction lAction, quint16 lAdminPort = 0, QHostAddress::SpecialAddress lAddress = QHostAddress::Any);
      void fControlClientServer(EServiceAction lAction, quint16 lClientPort = 0, QHostAddress::SpecialAddress lAddress = QHostAddress::Any);
      void fControlCommServer(EServiceAction lAction, quint16 lCommPort = 0, QHostAddress::SpecialAddress lAddress = QHostAddress::Any);
  };
}

#endif // NConnectionController_H
