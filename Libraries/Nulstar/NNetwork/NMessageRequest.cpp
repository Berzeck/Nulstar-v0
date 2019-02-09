#include "NMessageRequest.h"

namespace NulstarNS {
  NMessageRequest::NMessageRequest (const QString& lConnectionName, const QString& lMessageID, bool lRequestAck, quint64 lSubscriptionEventCounter, quint64 lSubscriptionPeriod,
                                        const QString& lSubscriptionRange, quint64 lResponseMaxSize, const QVariantMap& lRequestMethods, QObject *rParent)
                 : NMessage(lConnectionName, lMessageID, rParent), mRequestAck(lRequestAck), mResponseMaxSize(lResponseMaxSize), mSubscriptionEventCounter(lSubscriptionEventCounter), mSubscriptionPeriod(lSubscriptionPeriod),
                            mSubscriptionRange(lSubscriptionRange), mRequestMethods(lRequestMethods) {


  }

  QVariantMap NMessageRequest::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lRequestAckFieldName, mRequestAck);
    lMessageData.insert(lSubscriptionEventCounterFieldName, QString::number(mSubscriptionEventCounter));
    lMessageData.insert(lSubscriptionPeriodFieldName, QString::number(mSubscriptionPeriod));
    lMessageData.insert(lSubscriptionRangeFieldName, mSubscriptionRange);
    lMessageData.insert(lResponseMaxSizeFieldName, QString::number(mResponseMaxSize));
    lMessageData.insert(cFieldName_RequestMethods, mRequestMethods);
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

  if (lMessageObject.value(cMessageTypeFieldName).toString() != cTypeRequest) {
    qDebug("Message type is not 'Request'!");
    return false;
  }


  return true;
}

}
