#ifndef NWEBSOCKETSERVER_H
#define NWEBSOCKETSERVER_H

#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QObject>
#include <QWebSocketServer>
#include <QVersionNumber>

#include "NMessage.h"
#include "NMessageFactory.h"
#include "NNetwork.h"

QT_FORWARD_DECLARE_CLASS(NWebSocket)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NWebSocketServer : public QWebSocketServer {
    Q_OBJECT

    public:
      explicit NWebSocketServer(const QString& lName, const QString& lLabel, SslMode lSslMode, const QList<QVersionNumber>& lProtocolVersionsSupported, QObject* rParent = nullptr);
      virtual ~NWebSocketServer();

      bool fListen(const QHostAddress& lAddress = QHostAddress::Null, quint16 lPort = 0);
      QString fName() const { return mName; }
      quint16 fPort() const { return mPort; }
      int fMaxConnections() { return mMaxConnections; }
      void fRemoveConnections(const QList<qint64>& lConnectionIDList = QList<qint64>());
      int fTotalConnections() { return mConnections.size(); }
      void fSetName(const QString& lName);
      void fSetMaxConnections(int lMaxConnections) { mMaxConnections = lMaxConnections; }
      void fSetPort(quint16 lPort);
      void fSetBindAddress(const QHostAddress& lBindAddress);

    protected:
      bool fVersionSupported(const QJsonObject& lObjectMessage);
      void fProcessNegotiateConnection(const QJsonObject& lObjectMessage, NWebSocket* rConnection);

    private:
      int mMaxConnections;
      quint16 mPort;
      NMessageFactory lMessageFactory;
      QList<QVersionNumber> mProtocolVersionsSupported;
      QString mName;      
      QHostAddress mBindAddress;
      QMap<qint64, NWebSocket* > mConnections;
      QMap<qint64, QMap<QString, NMessage*> > mMessageQueue; // ConnectionID, <MessageID, Message>
      bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0) { return QWebSocketServer::listen(address, port); }
      void setServerName(const QString& serverName) { QWebSocketServer::setServerName(serverName); }

    private Q_SLOTS:
      void fOnNewConnection();
      void fProcessTextMessage(const QString& lMessage);
      void fSocketDisconnected();

    Q_SIGNALS:
      void fTextMessageArrived(QString lWebServerName, QString lMessage);
  };
}

#endif // NWEBSOCKETSERVER_H
