#ifndef NNOTIFICATION_H
#define NNOTIFICATION_H

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
class NETWORKSHARED_EXPORT NNotification : public NMessage {
    Q_OBJECT
public:
    explicit NNotification(const QString& lConnectionName,
        const QString& lMessageID = QString(),
        const QString& lNotificationAck = QString(),
        const QString& lNotificationType = QString(),
        const QString& lNotificationComment = QString(),
        const QVariantMap& lNotificationData = QVariantMap(),
        QObject* rParent = nullptr);

    QString fNotificationAck() const { return mNotificationAck; }
    QString fNotificationType() const { return mNotificationType; }
    QString fNotificationComment() const { return mNotificationComment; }
    QVariantMap fNotificationData() const { return mNotificationData; }

protected:
    QVariantMap fMessageData() const override;
    QString fMessageType() const override { return QString("Notification"); }

private:
    QString mNotificationAck;
    QString mNotificationType;
    QString mNotificationComment;
    QVariantMap mNotificationData;
};
}
#endif // NNOTIFICATION_H
