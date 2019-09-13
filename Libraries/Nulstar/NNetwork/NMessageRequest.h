#ifndef NMESSAGEREQUEST_H
#define NMESSAGEREQUEST_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  const QString lRequestAckFieldName("RequestAck");
  const QString cFieldName_SubscriptionEventCounter("SubscriptionEventCounter");
  const QString cFieldName_SubscriptionPeriod("SubscriptionPeriod");
  const QString lSubscriptionRangeFieldName("SubscriptionRange");
  const QString lResponseMaxSizeFieldName("ResponseMaxSize");
  const QString cFieldName_RequestMethods("RequestMethods");
  const QString cFieldName_TimeOut("TimeOut");
  class NETWORKSHARED_EXPORT NMessageRequest : public NMessage {
    Q_OBJECT

    public:
      explicit NMessageRequest(const QString& lConnectionName, const QString& lMessageID = QString(), bool lRequestAck = false, quint64 lSubscriptionEventCounter = 0, quint64 lSubscriptionPeriod = 0,
                               const QString& lSubscriptionRange = QString(), quint64 lResponseMaxSize = 0, const QVariantMap& lRequestMethods = QVariantMap(), const QString& lTimeOut = QString("10000"), QObject* rParent = nullptr);
      ~NMessageRequest() override {}

      bool fRequestAck() const { return mRequestAck; }
      quint64 fResponseMaxSize() { return mResponseMaxSize; }
      quint64 fSubscriptionEventCounter() { return mSubscriptionEventCounter; }
      quint64 fSubscriptionPeriod() { return mSubscriptionPeriod; }
      QString fSubscritionRange() { return mSubscriptionRange; }
      QVariantMap fRequestMethods() { return mRequestMethods; }

      bool fAddMethod(const QString& lMethodName);
      bool fAddParameter(const QString& lMethodName, const QString& lParameterName, const QVariant& lParameterValue);
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return cTypeRequest; }

    private:
      bool mRequestAck;
      quint64 mResponseMaxSize;
      quint64 mSubscriptionEventCounter;
      quint64 mSubscriptionPeriod;
      QString mSubscriptionRange;
      QString mTimeOut;
      QVariantMap mRequestMethods;
  };

  struct TMessageRequestToProcess {
      QString mWebSocketsServerName;
      QString mWebSocketID;
      QString mMessageID;
      QString mOriginalMethodName;
      QString mEffectiveMethodName;
      QVariantMap mParameters;
      quint64 mSubscriptionEventCounter;
      quint64 mSubscriptionPeriod;
      qint64 mMSecsSinceEpoch;
      quint64 mEventCounter;
      QString mTimeOut;
  };
} // namespace NulstarNS

#endif // NMESSAGEREQUEST_H
