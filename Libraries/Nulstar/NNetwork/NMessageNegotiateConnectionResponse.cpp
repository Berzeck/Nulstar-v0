#include "NMessageNegotiateConnectionResponse.h"

namespace NulstarNS {
  NMessageNegotiateConnectionResponse::NMessageNegotiateConnectionResponse(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, ENegotiationStatus lNegotiationStatus, const QString& lNegotiationComment, QObject *rParent)
                                     : NMessage(lConnectionName, lMessageID, rParent), mRequestID(lRequestID), mNegotiationStatus(lNegotiationStatus), mNegotiationComment(lNegotiationComment) {


  }

  QVariantMap NMessageNegotiateConnectionResponse::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(cNegotiationRequestIDFieldName, mRequestID);
    lMessageData.insert(cNegotiationStatusFieldName, mNegotiationStatus == ENegotiationStatus::eNegotiationError ? QString("0") : QString("1"));
    lMessageData.insert(cNegotiationCommentFieldName, mNegotiationComment);
    return lMessageData;
  }

  bool NMessageNegotiateConnectionResponse::fValidateMessageObject(const QJsonObject& lMessageObject) {
    if(!NMessage::fValidateMessageObject(lMessageObject)) {
      return false;
    }

    QJsonObject lDataObject = lMessageObject.value(cFieldName_MessageData).toObject();
    if(!lDataObject.contains(cNegotiationStatusFieldName)) {
      qDebug("Message received without 'NegotiationStatus' field!");
      return false;
    }
    int lNegotiationStatus = lDataObject.value(cNegotiationStatusFieldName).toInt();
    if((lNegotiationStatus !=  cNegotiationStatusSuccess) && (lNegotiationStatus !=  cNegotiationStatusFailure)) {
      qDebug("NegotiationStatus value out of scope!");
      return false;
    }
    if(!lDataObject.contains(cNegotiationCommentFieldName)) {
      qDebug("Message received without 'NegotiationComment' field!");
      return false;
    }
    if(!lDataObject.contains(cNegotiationRequestIDFieldName)) {
      qDebug("Message received without 'RequestID' field!");
      return false;
    }
    return NMessage::fValidateMessageObject(lMessageObject);
  }
}
