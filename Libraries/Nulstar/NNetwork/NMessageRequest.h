#ifndef NMESSAGEREQUEST_H
#define NMESSAGEREQUEST_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageRequest : public NMessage {
    Q_OBJECT

    public:
      explicit NMessageRequest(const QString& lConnectionName, const QString& lMessageID = QString(), bool lRequestAck = false, quint64 lSubscriptionEventCounter = 0, quint64 lSubscriptionPeriod = 0,
                               const QString& lSubscriptionRange = QString(), quint64 lResponseMaxSize = 0, const QVariantMap& lRequestMethods = QVariantMap(), QObject* rParent = nullptr);
      ~NMessageRequest() override {}

      bool fRequestAck() const { return mRequestAck; }
      quint64 fResponseMaxSize() { return mResponseMaxSize; }
      quint64 fSubscriptionEventCounter() { return mSubscriptionEventCounter; }
      quint64 fSubscriptionPeriod() { return mSubscriptionPeriod; }
      QString fSubscritionRange() { return mSubscriptionRange; }
      QVariantMap fRequestMethods() { return mRequestMethods; }

      bool fAddMethod(const QString& lMethodName);
      bool fAddParameter(const QString& lMethodName, const QString& lParameterName, const QVariant& lParameterValue);

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return cTypeRequest; }
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);
    private:
      bool mRequestAck;
      quint64 mResponseMaxSize;
      quint64 mSubscriptionEventCounter;
      quint64 mSubscriptionPeriod;
      QString mSubscriptionRange;
      QVariantMap mRequestMethods;
  };
} // namespace NulstarNS

#endif // NMESSAGEREQUEST_H
