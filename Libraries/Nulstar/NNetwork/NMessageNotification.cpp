#include "NMessageNotification.h"

namespace NulstarNS {
  const QString lNotificationAckFieldName("NotificationAck");
  const QString lNotificationTypeFieldName("NotificationType");
  const QString lNotificationCommentFieldName("NotificationComment");
  const QString lNotificationDataFieldName("NotificationData");

  NMessageNotification::NMessageNotification(const QString& lConnectionName, const QString& lMessageID, bool lNotificationAck, const QString& lNotificationType, const QString& lNotificationComment, const QVariantMap& lNotificationData, QObject* rParent)
               : NMessage(lConnectionName, lMessageID, rParent), mNotificationAck(lNotificationAck), mNotificationType(lNotificationType), mNotificationComment(lNotificationComment), mNotificationData(lNotificationData) {
  }

  QVariantMap NMessageNotification::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lNotificationAckFieldName, mNotificationAck);
    lMessageData.insert(lNotificationTypeFieldName, mNotificationType);
    lMessageData.insert(lNotificationCommentFieldName, mNotificationComment);
    lMessageData.insert(lNotificationDataFieldName, mNotificationData);
    return lMessageData;
  }
}
