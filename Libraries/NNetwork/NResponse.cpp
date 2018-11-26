#include "NResponse.h"

namespace NulstarNS {

const QString lRequestIDFieldName("RequestID");
const QString lResponseProcessingTimeFieldName("ResponseProcessingTime");
const QString lResponseStatusFieldName("ResponseStatus");
const QString lResponseCommentFieldName("ResponseComment");
const QString lResponseMaxSizeFieldName("ResponseMaxSize");
const QString lResponseDataFieldName("ResponseData");

NResponse::NResponse(const QString& lConnectionName, const QString& lMessageID, const QString& lRequestID, const QString& lResponseProcessingTime, const QString& lResponseStatus, const QString& lResponseComment, const QString& lResponseMaxSize, const QVariantMap& lResponseData, QObject* rParent)
    : NMessage(lConnectionName, lMessageID, rParent)
    , mRequestID(lRequestID)
    , mResponseProcessingTime(lResponseProcessingTime)
    , mResponseStatus(lResponseStatus)
    , mResponseComment(lResponseComment)
    , mResponseMaxSize(lResponseMaxSize)
    , mResponseData(lResponseData)
{
}

QVariantMap NResponse::fMessageData() const
{
    QVariantMap lMessageData;
    lMessageData.insert(lRequestIDFieldName, fRequestID());
    lMessageData.insert(lResponseProcessingTimeFieldName, fResponseProcessingTime());
    lMessageData.insert(lResponseStatusFieldName, fResponseStatus());
    lMessageData.insert(lResponseCommentFieldName, fResponseComment());
    lMessageData.insert(lResponseMaxSizeFieldName, fResponseMaxSize());
    lMessageData.insert(lResponseDataFieldName, fResponseData());
    return lMessageData;
}

} // namespace NulstarNS
