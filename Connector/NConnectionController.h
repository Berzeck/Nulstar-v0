#ifndef NCONNECTIONCONTROLLER_H
#define NCONNECTIONCONTROLLER_H

#include <QObject>
#include <Network.h>

namespace NulstarNS {
  class NNulstarWebSocketsServer;
  class NETWORKSHARED_EXPORT NConnectionController : public QObject {
    Q_OBJECT

    public:      
      explicit NConnectionController(quint8 lSecureMode, quint16 lAdminPort, quint16 lClientPort, quint16 lCommPort, quint8 lDebugLevel, QObject* rParent = nullptr);
      ~NConnectionController();

    private:      
      quint8 mSecureMode;
      quint16 mAdminPort;
      quint16 mClientPort;
      quint16 mCommPort;
      quint8 mDebugLevel;
      NNulstarWebSocketsServer* pWebAdminServer;
      NNulstarWebSocketsServer* pWebClientServer;
      NNulstarWebSocketsServer* pWebCommServer;

    public Q_SLOTS:
      void fControlAdminServer(bool lStartServer);
      void fControlClientServer(bool lStartServer);
      void fControlCommServer(bool lStartServer);
  };
}

#endif // NConnectionController_H
