#include "NMessageResponse.h"

namespace NulstarNS {
  const QString lRequestIDFieldName("RequestID");
  const QString lResponseProcessingTimeFieldName("ResponseProcessingTime");
  const QString lResponseStatusFieldName("ResponseStatus");
  const QString lResponseCommentFieldName("ResponseComment");
  const QString lResponseMaxSizeFieldName("ResponseMaxSize");
  const QString lResponseDataFieldName("ResponseData");

  NMessageResponse::NMessageResponse(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, const quint64 lResponseProcessingTime, const EResponseStatus lResponseStatus,
                                     const QString& lResponseComment, const quint64 lResponseMaxSize, const QVariantMap& lResponseData, QObject* rParent)
                  : NMessage(lConnectionName, lMessageID, rParent), mRequestID(lRequestID), mResponseProcessingTime(lResponseProcessingTime), mResponseStatus(lResponseStatus), mResponseComment(lResponseComment),
                    mResponseMaxSize(lResponseMaxSize), mResponseData(lResponseData) {

  }

  QVariantMap NMessageResponse::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lRequestIDFieldName, mRequestID);
    lMessageData.insert(lResponseProcessingTimeFieldName, QString::number(mResponseProcessingTime));
    lMessageData.insert(lResponseStatusFieldName, mResponseStatus == EResponseStatus::eResponseSuccessful ? QString("1") : QString("0"));
    lMessageData.insert(lResponseCommentFieldName, mResponseComment);
    lMessageData.insert(lResponseMaxSizeFieldName, QString::number(mResponseMaxSize));
    lMessageData.insert(lResponseDataFieldName, mResponseData);
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
} // namespace NulstarNS
