#ifndef NUNSUBSCRIBE_H
#define NUNSUBSCRIBE_H

#include "NMessage.h"

namespace NulstarNS {
class NETWORKSHARED_EXPORT NUnsubscribe : public NMessage {
    Q_OBJECT
public:
    explicit NUnsubscribe(const QString& lConnectionName,
        const QString& lMessageID = QString(),
        const QStringList& lUnscribeMethods = QStringList(),
        QObject* rParent = nullptr);
    ~NUnsubscribe() override {}

    QStringList fUnsubscribeMethods() const { return mUnscribeMethods; }

protected:
    QVariantMap fMessageData() const override;
    QString fMessageType() const override { return QString("Unsubscribe"); }

private:
    QStringList mUnscribeMethods;
};
} // namespace NulstarNS
#endif // NUNSUBSCRIBE_H
