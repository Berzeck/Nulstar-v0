#ifndef NMESSAGEREGISTERCOMPOUNDMETHOD_H
#define NMESSAGEREGISTERCOMPOUNDMETHOD_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageRegisterCompoundMethod : public NMessage {
    Q_OBJECT

    public:
      explicit NMessageRegisterCompoundMethod(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lCompoundMethodName = QString(), const QString& lCompoundMethodDescription = QString(),
                                              const QVariantMap& lCompoundMethods = QVariantMap(), QObject* rParent = nullptr);
      ~NMessageRegisterCompoundMethod() override {}

      QString fCompoundMethodName() const { return mCompoundMethodName; }
      QString fCompoundMethodDescription() const { return mCompoundMethodDescription; }
      QVariantMap fCompoundMethods() const { return mCompoundMethods; }

      bool fAddMethod(const QString& lMethodName);
      bool fAddParameter(const QString& lMethodName, const QString& lParameterName, const QVariant& lParameterValue);

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return QString("RegisterCompoundMethod"); }

    private:
      QString mCompoundMethodName;
      QString mCompoundMethodDescription;
      QVariantMap mCompoundMethods;
  };
} // namespace NulstarNS
#endif // NREGISTERCOMPOUNDMETHOD_H
