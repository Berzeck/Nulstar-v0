#include "NMessageNegotiateConnectionResponse.h"

namespace NulstarNS {
  const QString cNegotiationStatusFieldName("NegotiationStatus");
  const QString cNegotiationCommentFieldName("NegotiationComment");
  const int cNegotiationStatusSuccess = 1;
  const int cNegotiationStatusFailure = 0;

  NMessageNegotiateConnectionResponse::NMessageNegotiateConnectionResponse(const QString& lConnectionName, const QString& lMessageID, ENegotiationStatus lNegotiationStatus, const QString& lNegotiationComment, QObject *rParent)
                                     : NMessage(lConnectionName, lMessageID, rParent), mNegotiationStatus(lNegotiationStatus), mNegotiationComment(lNegotiationComment) {


  }

  QVariantMap NMessageNegotiateConnectionResponse::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(cNegotiationStatusFieldName, mNegotiationStatus == ENegotiationStatus::eNegotiationError ? QString("0") : QString("1"));
    lMessageData.insert(cNegotiationCommentFieldName, mNegotiationComment);
    return lMessageData;
  }

  bool NMessageNegotiateConnectionResponse::fValidateMessageObject(const QJsonObject& lMessageObject) {
    if(!lMessageObject.contains(cMessageTypeFieldName)) {
        qDebug("Message received without 'MessageType' field!");
        return false;
    }
    if (lMessageObject.value(cMessageTypeFieldName).toString() != cTypeNegotiateConnectionResponse){
        qDebug("Message type is not 'NegotiateConnectionResponse'!");
        return false;
    }
    if(!lMessageObject.contains(cNegotiationStatusFieldName)) {
      qDebug("Message received without 'NegotiationStatus' field!");
      return false;
    }
    int lNegotiationStatus = lMessageObject.value(cNegotiationStatusFieldName).toInt();
    if((lNegotiationStatus !=  cNegotiationStatusSuccess) || (lNegotiationStatus !=  cNegotiationStatusFailure)) {
      qDebug("NegotiationStatus value out of scope!");
      return false;
    }
    if(!lMessageObject.contains(cNegotiationCommentFieldName)) {
      qDebug("Message received without 'NegotiationComment' field!");
      return false;
    }
    return NMessage::fValidateMessageObject(lMessageObject);
  }
}
