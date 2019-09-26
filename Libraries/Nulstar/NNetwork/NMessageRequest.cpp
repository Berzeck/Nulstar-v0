#include "NMessageRequest.h"

namespace NulstarNS {
  NMessageRequest::NMessageRequest (const QString& lConnectionName, const QString& lMessageID, bool lRequestAck, quint64 lSubscriptionEventCounter, quint64 lSubscriptionPeriod,
                                        const QString& lSubscriptionRange, quint64 lResponseMaxSize, const QVariantMap& lRequestMethods, const QString& lTimeOut, QObject *rParent)
                 : NMessage(lConnectionName, lMessageID, rParent), mRequestAck(lRequestAck), mResponseMaxSize(lResponseMaxSize), mSubscriptionEventCounter(lSubscriptionEventCounter), mSubscriptionPeriod(lSubscriptionPeriod),
                            mSubscriptionRange(lSubscriptionRange), mTimeOut(lTimeOut), mRequestMethods(lRequestMethods) {


  }

  QVariantMap NMessageRequest::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lRequestAckFieldName, mRequestAck ? "1" : "0");
    lMessageData.insert(cFieldName_SubscriptionEventCounter, QString::number(mSubscriptionEventCounter));
    lMessageData.insert(cFieldName_SubscriptionPeriod, QString::number(mSubscriptionPeriod));
    lMessageData.insert(lSubscriptionRangeFieldName, mSubscriptionRange);
    lMessageData.insert(lResponseMaxSizeFieldName, QString::number(mResponseMaxSize));
    lMessageData.insert(cFieldName_RequestMethods, mRequestMethods);
  //  lMessageData.insert(cFieldName_TimeOut, mTimeOut);
    return lMessageData;
  }

  bool NMessageRequest::fAddMethod(const QString& lMethodName) {
    if(mRequestMethods.contains(lMethodName))
      return false;
    mRequestMethods.insert(lMethodName, QVariantMap());
    return true;
  }

  bool NMessageRequest::fAddParameter(const QString& lMethodName, const QString &lParameterName, const QVariant &lParameterValue) {
    if(!mRequestMethods.contains(lMethodName))
      return false;
    mRequestMethods[lMethodName].toMap()[lParameterName] = lParameterValue;
    return true;
  }

  bool NMessageRequest::fValidateMessageObject(const QJsonObject &lMessageObject) {
    if (!NMessage::fValidateMessageObject(lMessageObject)) {
      return false;
    }

    if(lMessageObject.value(cMessageTypeFieldName).toString() != cTypeRequest) {
      qWarning("Message type is not 'Request'!");
      return false;
    }

    QJsonValue lValue(lMessageObject.value(cFieldName_MessageData));
    if(lValue.isUndefined()) {
      qWarning("Request message does not have 'MessageData' field!");
      return false;
    }
    if(!lValue.isObject()) {
      qWarning("'MessageData' field does not contain a JSON object.");
      return false;
    }
    lValue = lValue.toObject().value(cFieldName_RequestMethods);
    if(!lValue.isObject()) {
      qWarning("'RequestMethods' field does not contain a JSON object.");
      return false;
    }
    return true;
  }
}
