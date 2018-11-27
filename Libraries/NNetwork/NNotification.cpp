#include "NNotification.h"

namespace NulstarNS {
const QString lNotificationAckFieldName("NotificationAck");
const QString lNotificationTypeFieldName("NotificationType");
const QString lNotificationCommentFieldName("NotificationComment");
const QString lNotificationDataFieldName("NotificationData");

NNotification::NNotification(const QString& lConnectionName, const QString& lMessageID, const QString& lNotificationAck, const QString& lNotificationType, const QString& lNotificationComment, const QVariantMap& lNotificationData, QObject* rParent)
    : NMessage(lConnectionName, lMessageID, rParent)
    , mNotificationAck(lNotificationAck)
    , mNotificationType(lNotificationType)
    , mNotificationComment(lNotificationComment)
    , mNotificationData(lNotificationData)
{
}

QVariantMap NNotification::fMessageData() const
{
    QVariantMap lMessageData;
    lMessageData.insert(lNotificationAckFieldName, fNotificationAck());
    lMessageData.insert(lNotificationTypeFieldName, fNotificationType());
    lMessageData.insert(lNotificationCommentFieldName, fNotificationComment());
    lMessageData.insert(lNotificationDataFieldName, fNotificationData());
    return lMessageData;
}

}
