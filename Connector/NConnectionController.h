#ifndef NCONNECTIONCONTROLLER_H
#define NCONNECTIONCONTROLLER_H

#include <QObject>
#include <NCoreService.h>
#include <Network.h>

namespace NulstarNS {
  class NWebSocketsServer;
  class NETWORKSHARED_EXPORT NConnectionController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NConnectionController(quint8 lSecureMode, quint8 lDebugLevel, QObject* rParent = nullptr);
      ~NConnectionController();

    private:      
      quint8 mSecureMode;
      quint8 mDebugLevel;
      NWebSocketsServer* pWebAdminServer;
      NWebSocketsServer* pWebClientServer;
      NWebSocketsServer* pWebCommServer;

    public Q_SLOTS:
      void fControlAdminServer(bool lStartServer, quint16 lAdminPort = 0);
      void fControlClientServer(bool lStartServer, quint16 lClientPort = 0);
      void fControlCommServer(bool lStartServer, quint16 lCommPort = 0);
  };
}

#endif // NConnectionController_H
