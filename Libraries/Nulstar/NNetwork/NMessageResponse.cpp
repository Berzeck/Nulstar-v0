#include "NMessageResponse.h"

namespace NulstarNS {
  const QString cRequestIDFieldName("RequestID");
  const QString cResponseProcessingTimeFieldName("ResponseProcessingTime");
  const QString cResponseStatusFieldName("ResponseStatus");
  const QString cResponseCommentFieldName("ResponseComment");
  const QString cResponseMaxSizeFieldName("ResponseMaxSize");
  const QString cResponseDataFieldName("ResponseData");
  const int cResponseStatusSuccess = 1;
  const int cResponseStatusFailure = 0;


  NMessageResponse::NMessageResponse(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, const quint64 lResponseProcessingTime, const EResponseStatus lResponseStatus,
                                     const QString& lResponseComment, const quint64 lResponseMaxSize, const QVariantMap& lResponseData, QObject *rParent)
                  : NMessage(lConnectionName, lMessageID, rParent), mRequestID(lRequestID), mResponseProcessingTime(lResponseProcessingTime), mResponseStatus(lResponseStatus), mResponseComment(lResponseComment),
                    mResponseMaxSize(lResponseMaxSize), mResponseData(lResponseData) {

  }

  QVariantMap NMessageResponse::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(cRequestIDFieldName, mRequestID);
    lMessageData.insert(cResponseProcessingTimeFieldName, QString::number(mResponseProcessingTime));
    lMessageData.insert(cResponseStatusFieldName, mResponseStatus == EResponseStatus::eResponseSuccessful ? QString("1") : QString("0"));
    lMessageData.insert(cResponseCommentFieldName, mResponseComment);
    lMessageData.insert(cResponseMaxSizeFieldName, QString::number(mResponseMaxSize));
    lMessageData.insert(cResponseDataFieldName, mResponseData);
    return lMessageData;
  }

  bool NMessageResponse::fAddMethod(const QString& lMethodName) {
    if(mResponseData.contains(lMethodName))
      return false;
    mResponseData.insert(lMethodName, QVariantMap());
    return true;
  }

  bool NMessageResponse::fAddParameter(const QString& lMethodName, const QString &lParameterName, const QVariant &lParameterValue) {
    if(!mResponseData.contains(lMethodName))
      return false;
    mResponseData[lMethodName].toMap()[lParameterName] = lParameterValue;
    return true;
  }

  bool NMessageResponse::fValidateMessageObject(const QJsonObject& lMessageObject) {
    if(!lMessageObject.contains(cMessageTypeFieldName)) {
        qDebug("Message received without 'MessageType' field!");
        return false;
    }
    if (lMessageObject.value(cMessageTypeFieldName).toString() != cTypeReponse){
        qDebug("Message type is not 'Response'!");
        return false;
    }
    if(!lMessageObject.contains(cRequestIDFieldName)) {
      qDebug("Message received without 'RequestID' field!");
      return false;
    }
    if(!lMessageObject.contains(cResponseProcessingTimeFieldName)) {
      qDebug("Message received without 'ResponseProcessingTime' field!");
      return false;
    }
    if(!lMessageObject.contains(cResponseStatusFieldName)) {
      qDebug("Message received without 'ResponseStatus' field!");
      return false;
    }
    int lResponseStatus = lMessageObject.value(cResponseStatusFieldName).toInt();
    if((lResponseStatus !=  cResponseStatusSuccess) || (lResponseStatus !=  cResponseStatusFailure)) {
      qDebug("ResponseStatus value out of scope!");
      return false;
    }
    if(!lMessageObject.contains(cResponseCommentFieldName)) {
      qDebug("Message received without 'ResponseComment' field!");
      return false;
    }
    if(!lMessageObject.contains(cResponseMaxSizeFieldName)) {
      qDebug("Message received without 'ResponseMaxSize' field!");
      return false;
    }
    if(!lMessageObject.contains(cResponseDataFieldName)) {
      qDebug("Message received without 'ResponseData' field!");
      return false;
    }
    return NMessage::fValidateMessageObject(lMessageObject);
  }
} // namespace NulstarNS
