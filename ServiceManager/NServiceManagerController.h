#ifndef NSERVICEMANAGERCONTROLLER_H
#define NSERVICEMANAGERCONTROLLER_H

#include <QHostAddress>
#include <QObject>
#include <NCoreService.h>
#include <Network.h>
#include <NWebSocketServer.h>

namespace NulstarNS {
  class NWebSocketServer;
  class NETWORKSHARED_EXPORT NServiceManagerController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NServiceManagerController(QWebSocketServer::SslMode lSslMode, NCoreService::ELogLevel lLogLevel, QObject* rParent = nullptr);
      ~NServiceManagerController();

    private:      
      NWebSocketServer* pWebCommServer;

    public Q_SLOTS:
      void fControlCommServer(EServiceAction lAction, quint16 lCommPort = 0, QHostAddress::SpecialAddress lAddress = QHostAddress::Any);
  };
}

#endif // NServiceManagerController_H
