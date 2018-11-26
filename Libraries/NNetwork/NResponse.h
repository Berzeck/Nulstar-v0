#ifndef NRESPONSE_H
#define NRESPONSE_H

#include "NMessage.h"

namespace NulstarNS {
class NETWORKSHARED_EXPORT NResponse : public NMessage {
    Q_OBJECT
public:
    explicit NResponse(const QString& lConnectionName,
        const QString& lMessageID = QString(),
        const QString& lRequestID = QString(),
        const QString& lResponseProcessingTime = QString(),
        const QString& lResponseStatus = QString(),
        const QString& lResponseComment = QString(),
        const QString& lResponseMaxSize = QString(),
        const QVariantMap& lResponseData = QVariantMap(),
        QObject* rParent = nullptr);
    ~NResponse() override {}

    QString fRequestID() const { return mRequestID; }
    QString fResponseProcessingTime() const { return mResponseProcessingTime; }
    QString fResponseStatus() const { return mResponseStatus; }
    QString fResponseComment() const { return mResponseComment; }
    QString fResponseMaxSize() const { return mResponseMaxSize; }
    QVariantMap fResponseData() const { return mResponseData; }

protected:
    QVariantMap fMessageData() const override;
    QString fMessageType() const override { return QString("Response"); }

private:
    QString mRequestID;
    QString mResponseProcessingTime;
    QString mResponseStatus;
    QString mResponseComment;
    QString mResponseMaxSize;
    QVariantMap mResponseData;
};
} // namespace NulstarNS

#endif // NRESPONSE_H
