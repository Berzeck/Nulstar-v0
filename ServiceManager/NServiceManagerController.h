#ifndef NSERVICEMANAGERCONTROLLER_H
#define NSERVICEMANAGERCONTROLLER_H

#include <QObject>
#include <NCoreService.h>
#include <Network.h>

namespace NulstarNS {
  class NWebSocketsServer;
  class NETWORKSHARED_EXPORT NServiceManagerController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NServiceManagerController(quint8 lSecureMode, quint8 lDebugLevel, QObject* rParent = nullptr);
      ~NServiceManagerController();

    private:      
      quint8 mSecureMode;
      quint8 mDebugLevel;
      NWebSocketsServer* pWebCommServer;

    public Q_SLOTS:
      void fControlCommServer(bool lStartServer, quint16 lCommPort = 0);
  };
}

#endif // NServiceManagerController_H
