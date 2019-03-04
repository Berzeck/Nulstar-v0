#include "NMessageNotification.h"

namespace NulstarNS {
  const QString cNotificationAckFieldName("NotificationAck");
  const QString cNotificationTypeFieldName("NotificationType");
  const QString cNotificationCommentFieldName("NotificationComment");
  const QString cNotificationDataFieldName("NotificationData");
  const int cNotificationAckWithNone = 0;
  const int cNotificationAckWithOneAckMsg = 1;

  NMessageNotification::NMessageNotification(const QString& lConnectionName, const QString& lMessageID, bool lNotificationAck, const QString& lNotificationType, const QString& lNotificationComment, const QVariantMap& lNotificationData, QObject *rParent)
               : NMessage(lConnectionName, lMessageID, rParent), mNotificationAck(lNotificationAck), mNotificationType(lNotificationType), mNotificationComment(lNotificationComment), mNotificationData(lNotificationData) {
  }

  QVariantMap NMessageNotification::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(cNotificationAckFieldName, mNotificationAck);
    lMessageData.insert(cNotificationTypeFieldName, mNotificationType);
    lMessageData.insert(cNotificationCommentFieldName, mNotificationComment);
    lMessageData.insert(cNotificationDataFieldName, mNotificationData);
    return lMessageData;
  }

  bool NMessageNotification::fValidateMessageObject(const QJsonObject& lMessageObject) {
    if (!NMessage::fValidateMessageObject(lMessageObject)) {
      return false;
    }

    if(!lMessageObject.contains(cMessageTypeFieldName)) {
        qDebug("Message received without 'MessageType' field!");
        return false;
    }
    if (lMessageObject.value(cMessageTypeFieldName).toString() != cTypeNotification){
        qDebug("Message type is not 'Notification'!");
        return false;
    }
    if(!lMessageObject.contains(cNotificationAckFieldName)) {
      qDebug("Message received without 'NotificationAck' field!");
      return false;
    }
    int lNotificationAck = lMessageObject.value(cNotificationAckFieldName).toInt();
    if((lNotificationAck !=  cNotificationAckWithNone) || (lNotificationAck !=  cNotificationAckWithOneAckMsg)) {
      qDebug("NotificationAck value out of scope!");
      return false;
    }
    if(!lMessageObject.contains(cNotificationTypeFieldName)) {
      qDebug("Message received without 'NotificationType' field!");
      return false;
    }
    if(!lMessageObject.contains(cNotificationCommentFieldName)) {
      qDebug("Message received without 'NotificationComment' field!");
      return false;
    }
    if(!lMessageObject.contains(cNotificationDataFieldName)) {
      qDebug("Message received without 'NotificationData' field!");
      return false;
    }
    return NMessage::fValidateMessageObject(lMessageObject);
  }

}
