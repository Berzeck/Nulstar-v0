#ifndef NMESSAGENEGOTIATECONNECTIONRESPONSE_H
#define NMESSAGENEGOTIATECONNECTIONRESPONSE_H

#include <QObject>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageNegotiateConnectionResponse : public NMessage {
    Q_OBJECT

    public:

      explicit NMessageNegotiateConnectionResponse(const QString& lConnectionName, const QString& lMessageID = QString(), QObject* rParent = nullptr, quint8 lNegotiationStatus = 0,const QString& lNegotiationComment = QString());
      ~NMessageNegotiateConnectionResponse() override {}

      quint8 fNegotiationStatus() const { return mNegotiationStatus; }
      QString fNegotiationComment() const { return mNegotiationComment; }

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return QString("NegotiateConnectionResponse"); }

    private:
      quint8 mNegotiationStatus;
      QString mNegotiationComment;
  };
}

#endif // NMESSAGENEGOTIATECONNECTIONRESPONSE_H
