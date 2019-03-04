#include <QDateTime>
#include "NMessageResponse.h"

namespace NulstarNS {
  NMessageResponse::NMessageResponse(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, const qint64 lResponseProcessingTime, const EResponseStatus lResponseStatus,
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

  qint64 NMessageResponse::fCalculateResponseProccessingTime(qint64 lStartingTimestamp) {
    qint64 lCurrentMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch() - lStartingTimestamp);
    return lCurrentMSecsSinceEpoch;
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
    if(!NMessage::fValidateMessageObject(lMessageObject)) {
      return false;
    }

    QJsonObject lDataObject = lMessageObject.value(cFieldName_MessageData).toObject();
    if(lMessageObject.value(cMessageTypeFieldName).toString() != cTypeReponse){
      qDebug("Message type is not 'Response'!");
      return false;
    }    
    if(!lDataObject.contains(cRequestIDFieldName)) {
      qDebug("Message received without 'RequestID' field!");
      return false;
    }
    if(!lDataObject.contains(cResponseProcessingTimeFieldName)) {
      qDebug("Message received without 'ResponseProcessingTime' field!");
      return false;
    }
    if(!lDataObject.contains(cResponseStatusFieldName)) {
      qDebug("Message received without 'ResponseStatus' field!");
      return false;
    }
    int lResponseStatus = lDataObject.value(cResponseStatusFieldName).toInt();
    if((lResponseStatus !=  cResponseStatusSuccess) && (lResponseStatus !=  cResponseStatusFailure)) {
      qDebug("ResponseStatus value out of scope!");
      return false;
    }
    if(!lDataObject.contains(cResponseCommentFieldName)) {
      qDebug("Message received without 'ResponseComment' field!");
      return false;
    }
    if(!lDataObject.contains(cResponseMaxSizeFieldName)) {
      qDebug("Message received without 'ResponseMaxSize' field!");
      return false;
    }
    if(!lDataObject.contains(cResponseDataFieldName)) {
      qDebug("Message received without 'ResponseData' field!");
      return false;
    }
    return NMessage::fValidateMessageObject(lMessageObject);
  }
} // namespace NulstarNS
