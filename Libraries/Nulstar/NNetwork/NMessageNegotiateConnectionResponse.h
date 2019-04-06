#ifndef NMESSAGENEGOTIATECONNECTIONRESPONSE_H
#define NMESSAGENEGOTIATECONNECTIONRESPONSE_H

#include <QObject>
#include <QString>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  const QString cNegotiationStatusFieldName("NegotiationStatus");
  const QString cNegotiationCommentFieldName("NegotiationComment");
  const int cNegotiationStatusSuccess = 1;
  const int cNegotiationStatusFailure = 0;
  const QString cNegotiationRequestIDFieldName("RequestID");

  class NETWORKSHARED_EXPORT NMessageNegotiateConnectionResponse : public NMessage {
    Q_OBJECT

    public:
      enum class ENegotiationStatus { eNegotiationError = 0, eNegotiationSuccessful = 1 };

      explicit NMessageNegotiateConnectionResponse(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lRequestID = QString(), ENegotiationStatus lNegotiationStatus = ENegotiationStatus::eNegotiationSuccessful, const QString& lNegotiationComment = QString(), QObject* rParent = nullptr);
      ~NMessageNegotiateConnectionResponse() override {}

      ENegotiationStatus fNegotiationStatus() const { return mNegotiationStatus; }
      QString fNegotiationComment() const { return mNegotiationComment; }
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);
      QString fRequestID() const { return mRequestID; }

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return cTypeNegotiateConnectionResponse; }

    private:
      QString mRequestID;
      ENegotiationStatus mNegotiationStatus;
      QString mNegotiationComment;
  };
}

#endif // NMESSAGENEGOTIATECONNECTIONRESPONSE_H
