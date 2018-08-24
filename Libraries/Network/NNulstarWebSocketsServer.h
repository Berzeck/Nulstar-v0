#ifndef NNULSTARWEBSOCKETSSERVER_H
#define NNULSTARWEBSOCKETSSERVER_H

#include <QHostAddress>
#include <QObject>
#include <QWebSocketServer>
#include "Network.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NNulstarWebSocketsServer : public QWebSocketServer {
    Q_OBJECT

    public:
      explicit NNulstarWebSocketsServer(const QString& lName, SslMode lSecureMode, QObject* rParent = nullptr);
      ~NNulstarWebSocketsServer();

      bool fListen(const QHostAddress& lAddress = QHostAddress::Any, quint16 lPort = 0);

    private:
      QList<QWebSocket* > mConnections;

    private Q_SLOTS:
      void fOnNewConnection();
      void fProcessTextMessage(QString lMessage);
      void fProcessBinaryMessage(QByteArray lMessage);
      void fSocketDisconnected();
  };
}

#endif // NNULSTARWEBSOCKETSSERVER_H
