#include "NMessageAck.h"

namespace NulstarNS {
  const QString lRequestIDFieldName("RequestID");

  NMessageAck::NMessageAck(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, QObject* rParent)
             : NMessage(lConnectionName, lMessageID, rParent), mRequestID(lRequestID) {

  }

  QVariantMap NMessageAck::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lRequestIDFieldName, mRequestID);
    return lMessageData;
  }
}
