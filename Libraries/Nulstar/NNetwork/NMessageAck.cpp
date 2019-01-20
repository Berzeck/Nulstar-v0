#include "NMessageAck.h"

namespace NulstarNS {
  const QString cRequestIDFieldName("RequestID");

  NMessageAck::NMessageAck(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, QObject* rParent)
             : NMessage(lConnectionName, lMessageID, rParent), mRequestID(lRequestID) {

  }

  QVariantMap NMessageAck::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(cRequestIDFieldName, mRequestID);
    return lMessageData;
  }

  bool fValidateMessageObject(const QJsonObject& lMessageObject) {
    if(!lMessageObject.contains(cRequestIDFieldName)) {
      qDebug("Message received without 'RequestID' field!");
      return false;
    }
    return NMessage::fValidateMessageObject(lMessageObject);
  }

}
