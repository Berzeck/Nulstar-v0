#ifndef NMESSAGEUNREGISTERCOMPOUNDMETHOD_H
#define NMESSAGEUNREGISTERCOMPOUNDMETHOD_H

#include <QObject>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageUnregisterCompoundMethod : public NMessage {
    Q_OBJECT

    public:

      explicit NMessageUnregisterCompoundMethod(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lUnregisterCompoundMethodName = QString(), QObject* rParent = nullptr);
      ~NMessageUnregisterCompoundMethod() override {}

      QString fUnregisterCompoundMethodName() const { return mUnregisterCompoundMethodName; }

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return QString("UnregisterCompoundMethod"); }

    private:
      QString mUnregisterCompoundMethodName;
  };
}

#endif // NMESSAGEUNREGISTERCOMPOUNDMETHOD_H
