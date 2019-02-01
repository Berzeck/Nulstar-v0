#ifndef NMESSAGEACK_H
#define NMESSAGEACK_H

#include <QObject>
#include <QString>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageAck : public NMessage {
    Q_OBJECT

    public:
      explicit NMessageAck(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lRequestID = QString(), QObject* rParent = nullptr);
      ~NMessageAck() override {}

      QString fRequestID() const { return mRequestID; }
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return cTypeAck; }

    private:
      QString mRequestID;
  };
}

#endif // NMESSAGEACK_H
