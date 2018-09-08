#ifndef NCONNECTIONCONTROLLER_H
#define NCONNECTIONCONTROLLER_H

#include <QHostAddress>
#include <QObject>
#include <NCoreService.h>

namespace NulstarNS {
  class NWebSocketServer;
  class NConnectionController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NConnectionController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0, quint16 lAdminPort = 0, quint16 lClientPort = 0,
                                     QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NConnectionController() {}

    public Q_SLOTS:
      API_PRIVATE_FUNCTION void aFunction(int lParameter) { Q_UNUSED(lParameter); }
  };
}

#endif // NConnectionController_H
