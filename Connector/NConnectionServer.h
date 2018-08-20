#ifndef NConnectionServer_H
#define NConnectionServer_H

#include <QObject>
#include "Connector.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace NulstarNS {
  class CONNECTORSHARED_EXPORT NConnectionServer : public QObject {
    Q_OBJECT

    public:      
      explicit NConnectionServer(bool lSecureMode, quint16 lPort, quint8 lDebugLevel = 0, QObject* rParent = nullptr);
      ~NConnectionServer();

    private:      
      bool mSecureMode;
      quint8 mDebugLevel;
      QList<QWebSocket* > mClients;
      QWebSocketServer* pWebSocketServer;

    private Q_SLOTS:
      void fOnNewConnection();
      void fProcessTextMessage(QString lMessage);
      void fProcessBinaryMessage(QByteArray lMessage);
      void fSocketDisconnected();

    Q_SIGNALS:
      void sClosed();
  };
}

#endif // NConnectionServer_H
