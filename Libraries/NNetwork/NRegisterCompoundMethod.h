#ifndef NREGISTERCOMPOUNDMETHOD_H
#define NREGISTERCOMPOUNDMETHOD_H

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
class NETWORKSHARED_EXPORT NRegisterCompoundMethod : public NMessage {
    Q_OBJECT
public:
    explicit NRegisterCompoundMethod(const QString& lConnectionName,
        const QString& lMessageID = QString(),
        const QString& lCompoundMethodName = QString(),
        const QString& lCompoundMethodDescription = QString(),
        const QVariantMap& lCompoundMethods = QVariantMap(),
        QObject* rParent = nullptr);

    QString fCompoundMethodName() const { return mCompoundMethodName; }
    QString fCompoundMethodDescription() const { return mCompoundMethodDescription; }
    QVariantMap fCompoundMethods() const { return mCompoundMethods; }

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
