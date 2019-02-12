#ifndef NTESTERMESSAGE_H
#define NTESTERMESSAGE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QTest>
#include <NWebSocket.h>
#include "NCoreConstants.h"

namespace NulstarNS {
  class NTesterMessage : public NWebSocket {
    Q_OBJECT

    public:     
      explicit NTesterMessage(const QString& lProtocolVersion, const QUrl& lConnectionUrl, quint8 lConnectionRetryInterval = 0, QWebSocket* rSocket = nullptr, QObject* rParent = nullptr);
      virtual ~NTesterMessage() {}

      void fConnectServiceManager();
      void fSendBulkRequestMessage();

    protected:

    public Q_SLOTS:
      void fOnConnectionStateChanged(NWebSocket::EConnectionState lNewState);

  };
}


#endif // NTESTERMESSAGE_H
