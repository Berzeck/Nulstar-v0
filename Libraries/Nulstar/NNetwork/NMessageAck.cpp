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
    if (!NMessage::fValidateMessageObject(lMessageObject)) {
      return false;
    }

    if(!lMessageObject.contains(cMessageTypeFieldName)) {
        qDebug("Message received without 'MessageType' field!");
        return false;
    }
    if (lMessageObject.value(cMessageTypeFieldName).toString() != cTypeAck) {
        qDebug("Message type is not 'Ack'!");
        return false;
    }
    if(!lMessageObject.contains(cRequestIDFieldName)) {
      qDebug("Message received without 'RequestID' field!");
      return false;
    }
    return NMessage::fValidateMessageObject(lMessageObject);
  }

}
