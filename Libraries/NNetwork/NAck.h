#ifndef NACK_H
#define NACK_H

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {

class NETWORKSHARED_EXPORT NAck : public NMessage {
    Q_OBJECT
public:
    explicit NAck(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lRequestID = QString(), QObject* rParent = nullptr);

    QString fRequestID() const { return mRequestID; }

protected:
    QVariantMap fMessageData() const override;
    QString fMessageType() const override { return QString("Ack"); }

private:
    QString mRequestID;
};

} // namespace NulstarNS
#endif // NACK_H
