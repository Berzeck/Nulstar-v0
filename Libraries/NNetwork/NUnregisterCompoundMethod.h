#ifndef NUNREGISTERCOMPOUNDMETHOD_H
#define NUNREGISTERCOMPOUNDMETHOD_H

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
class NUnregisterCompoundMethod : public NMessage {
    Q_OBJECT
public:
    explicit NUnregisterCompoundMethod(const QString& lConnectionName,
        const QString& lMessageID = QString(),
        const QString& lUnregisterCompoundMethodName = QString(),
        QObject* rParent = nullptr);

    QString fUnregisterCompoundMethodName() const { return mUnregisterCompoundMethodName; }

protected:
    QVariantMap fMessageData() const override;
    QString fMessageType() const override { return QString("UnregisterCompoundMethod"); }

private:
    QString mUnregisterCompoundMethodName;
};
} // namespace NulstarNS
#endif // NUNREGISTERCOMPOUNDMETHOD_H
