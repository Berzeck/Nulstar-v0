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
      enum class ENegotiationStatus { eNegotiationError = 0, eNegotiationSuccessful = 1 };

      explicit NMessageNegotiateConnectionResponse(const QString& lConnectionName, const QString& lMessageID = QString(), ENegotiationStatus lNegotiationStatus = ENegotiationStatus::eNegotiationSuccessful, const QString& lNegotiationComment = QString(), QObject* rParent = nullptr);
      ~NMessageNegotiateConnectionResponse() override {}

      ENegotiationStatus fNegotiationStatus() const { return mNegotiationStatus; }
      QString fNegotiationComment() const { return mNegotiationComment; }

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return QString("NegotiateConnectionResponse"); }

    private:
      ENegotiationStatus mNegotiationStatus;
      QString mNegotiationComment;
  };
}

#endif // NMESSAGENEGOTIATECONNECTIONRESPONSE_H
