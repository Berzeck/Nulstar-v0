#include "NAck.h"

namespace NulstarNS {
const QString lRequestIDFieldName("RequestID");

NAck::NAck(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, QObject* rParent)
    : NMessage(lConnectionName, lMessageID, rParent)
    , mRequestID(lRequestID)
{
}

QVariantMap NAck::fMessageData() const
{
    QVariantMap lMessageData;
    lMessageData.insert(lRequestIDFieldName, fRequestID());
    return lMessageData;
}

}
