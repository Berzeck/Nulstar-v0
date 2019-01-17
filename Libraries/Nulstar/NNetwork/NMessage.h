#ifndef NMESSAGE_H
#define NMESSAGE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QVariantMap>

#include "NNetwork.h"

namespace NulstarNS {
  const QString cMessageIDFieldName("MessageID");
  const QString cTimeStampFieldName("Timestamp");
  const QString cTimeZoneFieldName("TimeZone");
  const QString cMessageTypeFieldName("MessageType");
  const QString cMessageDataFieldName("MessageData");

  const QString cTypeAck("Ack");
  const QString cTypeNegotiateConnection("NegotiateConnection");
  const QString cTypeNegotiateConnectionResponse("NegotiateConnectionResponse");
  const QString cTypeNotification("Notification");
  const QString cTypeRegisterCompoundMethod("RegisterCompoundMethod");
  const QString cTypeRequest("Request");
  const QString cTypeReponse("Response");
  const QString cTypeUnregisterCompoundMethod("UnregisterCompoundMethod");

  class NETWORKSHARED_EXPORT NMessage : public QObject {
    Q_OBJECT

    public:
      enum class EMessageStatus { eAwaitingDelivery = 1, eWithErrorAndAwaitingDelivery = 2, eWithErrorAndWitheld = 3, eAwaitingAck = 4, eAwaitingResponse = 5, eAwaitingAckAndResponse = 6, eSent = 7 };

      explicit NMessage(const QString& lConnectionName, const QString& lMessageID = QString(), QObject* rParent = nullptr);      
      virtual ~NMessage() {}

      QString fConnectionName() const { return mConnectionName; }
      QString fMessageID() const { return mMessageID; }
      EMessageStatus fStatus() const { return mStatus; }
      qint64 fTimeStamp() const { return mTimeStamp; }
      qint32 fTimeZone() const { return mTimeZone; }
      virtual QString fToJsonString(QJsonDocument::JsonFormat lFormat = QJsonDocument::Compact) const;
      void fSetStatus(const EMessageStatus lStatus) { mStatus = lStatus; }
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);

    protected:
      virtual QVariantMap fMessageData() const = 0;
      virtual QString fMessageType() const = 0;

      static qint64 mMessageIDPrefix;
      static qint64 mMessageIDSuffix;

    private:
      QString mConnectionName;
      QString mMessageID;
      EMessageStatus mStatus;
      qint64 mTimeStamp;
      qint32 mTimeZone;
  };
}

#endif // NMESSAGE_H
