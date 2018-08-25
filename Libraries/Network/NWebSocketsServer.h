#ifndef NWEBSOCKETSSERVER_H
#define NWEBSOCKETSSERVER_H

#include <QHostAddress>
#include <QObject>
#include <QWebSocketServer>
#include "Network.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NWebSocketsServer : public QWebSocketServer {
    Q_OBJECT

    public:
      explicit NWebSocketsServer(const QString& lName, SslMode lSecureMode, QObject* rParent = nullptr);
      virtual ~NWebSocketsServer();

      bool fListen(const QHostAddress& lAddress = QHostAddress::Any, quint16 lPort = 0);

    private:
      QList<QWebSocket* > mConnections;      
      bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0) { return QWebSocketServer::listen(address, port); }

    private Q_SLOTS:
      void fOnNewConnection();
      void fProcessTextMessage(QString lMessage);
      void fProcessBinaryMessage(QByteArray lMessage);
      void fSocketDisconnected();
  };
}

#endif // NNULSTARWEBSOCKETSSERVER_H
