#ifndef NWEBSOCKETSERVER_H
#define NWEBSOCKETSERVER_H

#include <QHostAddress>
#include <QObject>
#include <QWebSocketServer>
#include "Network.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NWebSocketServer : public QWebSocketServer {
    Q_OBJECT

    public:
      explicit NWebSocketServer(const QString& lName, SslMode lSslMode, QObject* rParent = nullptr);
      virtual ~NWebSocketServer();

      bool fListen(const QHostAddress& lAddress = QHostAddress::Any, quint16 lPort = 0);
      void fSetPort(quint16 lPort);

    private:
      quint16 mPort;
      QList<QWebSocket* > mConnections;
      bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0) { return QWebSocketServer::listen(address, port); }

    private Q_SLOTS:
      void fOnNewConnection();
      void fProcessTextMessage(QString lMessage);
      void fProcessBinaryMessage(QByteArray lMessage);
      void fSocketDisconnected();
  };
}

#endif // NNULSTARWEBSOCKETSERVER_H
