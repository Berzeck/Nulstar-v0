#ifndef NMESSAGENOTIFICATION_H
#define NMESSAGENOTIFICATION_H

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageNotification : public NMessage {
    Q_OBJECT

    public:
      explicit NMessageNotification(const QString& lConnectionName,  const QString& lMessageID = QString(), bool lNotificationAck = false, const QString& lNotificationType = QString(), const QString& lNotificationComment = QString(),
                                    const QVariantMap& lNotificationData = QVariantMap(), QObject* rParent = nullptr);
      ~NMessageNotification() override {}

      bool fNotificationAck() const { return mNotificationAck; }
      QString fNotificationType() const { return mNotificationType; }
      QString fNotificationComment() const { return mNotificationComment; }
      QVariantMap fNotificationData() const { return mNotificationData; }
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return cTypeNotification; }

    private:
      bool mNotificationAck;
      QString mNotificationType;
      QString mNotificationComment;
      QVariantMap mNotificationData;
  };
}
#endif // NNOTIFICATION_H
